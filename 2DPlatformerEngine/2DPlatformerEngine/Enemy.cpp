#include "Enemy.h"
#include "Window.h"

Enemy::Enemy()
{	
}

void Enemy::loadEnemy(int x, int y, int enemyType)
{
	posX = x;
	posY = y;

	this->enemyType = enemyType;
	switch (enemyType)
	{
	case ENEMY_FLYING:
		enemyTexture = Window::LoadImage(ENEMY_TEXTURE_FLYER);
		moveDirection = ENEMY_MOVE_UP;

		enemyHeight = ENEMY_FLYER_HEIGHT;
		enemyWidth = ENEMY_FLYER_WIDTH;

		maxVelX = 0;
		maxVelY = 50;

		isFalling = false;
		break;

	case ENEMY_WALKING:
		enemyTexture = Window::LoadImage(ENEMY_TEXTURE_WALKER);
		moveDirection = ENEMY_MOVE_LEFT;

		enemyHeight = ENEMY_WALKER_HEIGHT;
		enemyWidth = ENEMY_WALKER_WIDTH;

		maxVelX = 20;
		maxVelY = 40;

		isFalling = true;
		break;

	default:
		break;
	}

	internalVelX = 0;
	internalVelY = 0;

	updateSpeed();

	currentClip = 0;

	// Creates the clips for the enemy's animation
	for (int i = 0; i < ENEMY_WALK_CLIPS; i++)
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

void Enemy::updateTexture()
{
	switch (enemyType)
	{
	case ENEMY_FLYING:
		internalClipCounter = internalClipCounter % (ENEMY_WALK_CLIPS * 10);
		currentClip = internalClipCounter / 10;
		break;

	case ENEMY_WALKING:
		internalClipCounter = internalClipCounter % (ENEMY_WALK_CLIPS * 10 * 2);
		currentClip = internalClipCounter / 20;
		break;

	default:
		break;
	}

	internalClipCounter++;
}

void Enemy::updateSpeed()
{
	// If the enemy is falling, don't move horizontally
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