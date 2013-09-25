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
	case ENEMY_HARPY:
		enemyTexture = Window::LoadImage("media/EnemyHarpy64px.png");
		moveDirection = MOVE_UP;

		enemyHeight = 64;
		enemyWidth = 64;

		maxVelX = 0;
		maxVelY = 50;

		isFalling = false;
		break;

	case ENEMY_GRIZZLY:
		enemyTexture = Window::LoadImage("media/EnemyGrizzly64px.png");
		moveDirection = MOVE_LEFT;

		enemyHeight = 64;
		enemyWidth = 36;

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
	for (int i = 0; i < 4; i++)
	{
		enemyAnimationClips[i].x = i * enemyWidth;
		enemyAnimationClips[i].y = 0;
		enemyAnimationClips[i].h = enemyHeight;
		enemyAnimationClips[i].w = enemyWidth;
	}

	enemyHeight -= 2;
	enemyWidth -= 2;
}

void Enemy::updateTexture()
{
	switch (enemyType)
	{
	case ENEMY_HARPY:
		internalClipCounter = internalClipCounter % (maxVelY - 10);
		currentClip = internalClipCounter / 10;
		break;

	case ENEMY_GRIZZLY:
		internalClipCounter = internalClipCounter % (maxVelX * 4);
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
		internalVelX = 0;
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