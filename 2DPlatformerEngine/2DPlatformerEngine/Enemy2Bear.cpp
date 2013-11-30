#include "Enemy2Bear.h"

Enemy2Bear::Enemy2Bear()
{
}

void Enemy2Bear::loadEnemy(int x, int y, int height, int width, int maxAnimationClips, string texture)
{
	posX = x;
	posY = y;

	this->enemyType = ENEMY_WALKING;

	enemyTexture = Window::LoadImage(texture);
	moveDirection = ENEMY_MOVE_LEFT;

	enemyHeight = height;
	enemyWidth = width;

	maxVelX = 20;
	maxVelY = 40;

	isFalling = true;

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

void Enemy2Bear::move(Map map)
{
	// Move the walker horizontally based on its moveDirection + velocity.
	for (int movement = 0; movement < velX; movement++)
	{
		if (moveDirection == ENEMY_MOVE_RIGHT)
		{
			// Check for collision
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX + 1, posY,
				collisionPoints);
			int collisionType = checkCollision(map);
			if (collisionType > 0) {
				moveDirection = ENEMY_MOVE_LEFT;
				break;
			}

			posX++;
		}
		else if (moveDirection == ENEMY_MOVE_LEFT)
		{
			// Check for collision
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX - 1, posY,
				collisionPoints);
			int collisionType = checkCollision(map);
			if (collisionType > 0) {
				moveDirection = ENEMY_MOVE_RIGHT;
				break;
			}

			posX--;
		}
	}

	// Make the walker fall
	for (int movement = 0; movement < velY; movement++)
	{
		// Check for collision
		hc.calculateCollisionPoints(getHeight(), getWidth(),
			posX, posY + 1,
			collisionPoints);
		int collisionType = checkCollision(map);
		if (collisionType > 0) {
			isFalling = false;
			break;
		}

		isFalling = true;
		posY++;
	}
}

void Enemy2Bear::updateTexture()
{
	internalClipCounter = internalClipCounter % (ENEMY_WALK_CLIPS * 10 * 2);
	currentClip = internalClipCounter / 20;

	internalClipCounter++;
}