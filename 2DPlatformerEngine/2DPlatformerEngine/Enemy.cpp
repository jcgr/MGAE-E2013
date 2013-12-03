#include "Enemy.h"
#include "Window.h"

Enemy::Enemy()
{
}

void Enemy::loadEnemy(int x, int y)
{
	load(x, y, 64, 64, 1, ENEMY_TEXTURE_DEFAULT);

	moveDirection = ENEMY_MOVE_NOWHERE;

	maxVelX = 0;
	maxVelY = 0;

	isFalling = false;
}

void Enemy::move(Map map)
{
	// Don't move by default.
}

void Enemy::updateTexture()
{
	internalClipCounter = internalClipCounter % (maxClips * 10);
	currentClip = internalClipCounter / 10;

	internalClipCounter++;
}

SDL_RendererFlip Enemy::getFlip()
{
	return SDL_FLIP_NONE;
}

void Enemy::updateSpeed()
{
	// If the Enemy is falling, don't move horizontally
	if (isFalling) {
		if (internalVelX > 0) {
			internalVelX--;
		}
	}
	else {
		if (internalVelX < maxVelX) {
			internalVelX++;
		}
	}

	// Increase y velocity. Nothing decreases it, but /shrug
	if (internalVelY < maxVelY) {
		internalVelY++;
	}

	velX = internalVelX / 10;
	velY = internalVelY / 10;
}

SDL_Rect Enemy::getCurrentAnimationClip()
{
	if (isFalling) {
		return enemyAnimationClips[0];
	}

	return enemyAnimationClips[currentClip];
}

SDL_Texture* Enemy::getCurrentTexture()
{
	return enemyTexture;
}

int Enemy::getHeight()
{
	return enemyHeight;
}

int Enemy::getWidth()
{
	return enemyWidth;
}

int Enemy::checkCollision(Map map)
{
	int tileType = 0;

	// Iterate over all collision points to check for collision with the map
	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		SDL_Point tempPoint = collisionPoints[i];
		int tempCollisionType = map.getTile(tempPoint.x, tempPoint.y);

		if (tempCollisionType == COLLISION_GOAL) {
			// Do nothing 
		}
		else if (tempCollisionType != COLLISION_EMPTY) {
			tileType = tempCollisionType;
		}
	}

	return tileType;
}

void Enemy::load(int x, int y, int height, int width, int maxAnimationClips, string texture)
{
	posX = x;
	posY = y;

	enemyTexture = Window::LoadImage(texture);

	enemyHeight = height;
	enemyWidth = width;

	maxVelX = 0;
	maxVelY = 50;

	internalVelX = 0;
	internalVelY = 0;

	updateSpeed();

	currentClip = 0;
	maxClips = maxAnimationClips;

	// Creates the clips for the enemy's animation
	for (int i = 0; i < maxClips; i++)
	{
		enemyAnimationClips[i].x = i * enemyWidth;
		enemyAnimationClips[i].y = 0;
		enemyAnimationClips[i].h = enemyHeight;
		enemyAnimationClips[i].w = enemyWidth;
	}

	// Reduce player size a little to allow for running through holes
	// that are only 1 tile wide.
	enemyHeight -= 2;
	enemyWidth -= 2;
}