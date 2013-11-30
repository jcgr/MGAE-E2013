#include "Enemy2Harpy.h"

Enemy2Harpy::Enemy2Harpy()
{
}

void Enemy2Harpy::loadEnemy(int x, int y, int height, int width, int maxAnimationClips, string texture)
{
	posX = x;
	posY = y;

	this->enemyType = ENEMY_FLYING;

	enemyTexture = Window::LoadImage(texture);
	moveDirection = ENEMY_MOVE_UP;

	enemyHeight = height;
	enemyWidth = width;

	maxVelX = 0;
	maxVelY = 50;

	isFalling = false;

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

void Enemy2Harpy::move(Map map)
{
	for (int movement = 0; movement < velY; movement++)
	{
		if (moveDirection == ENEMY_MOVE_UP)
		{
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX, posY - 1,
				collisionPoints);
			int collisionType = checkCollision(map);
			if (collisionType > 0) {
				moveDirection = ENEMY_MOVE_DOWN;
				break;
			}

			posY--;
		}
		else if (moveDirection == ENEMY_MOVE_DOWN)
		{
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX, posY + 1,
				collisionPoints);
			int collisionType = checkCollision(map);
			if (collisionType > 0) {
				moveDirection = ENEMY_MOVE_UP;
				break;
			}

			posY++;
		}
	}
}

void Enemy2Harpy::updateTexture()
{
	internalClipCounter = internalClipCounter % (maxClips * 10 * 2);
	currentClip = internalClipCounter / 20;

	internalClipCounter++;
}