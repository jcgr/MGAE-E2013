#include "EnemyBear.h"

EnemyBear::EnemyBear()
{
}

void EnemyBear::loadEnemy(int x, int y)
{
	this->load(x, y, ENEMY_WALKER_HEIGHT, ENEMY_WALKER_WIDTH, ENEMY_WALK_CLIPS, ENEMY_TEXTURE_WALKER);

	moveDirection = ENEMY_MOVE_LEFT;

	maxVelX = 20;
	maxVelY = 40;

	isFalling = true;
}

void EnemyBear::move(Map map)
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

void EnemyBear::updateTexture()
{
	internalClipCounter = internalClipCounter % (maxClips * 10 * 2);
	currentClip = internalClipCounter / 20;

	internalClipCounter++;
}

SDL_RendererFlip EnemyBear::getFlip()
{
	if (this->moveDirection == ENEMY_MOVE_LEFT) {
		return SDL_FLIP_HORIZONTAL;
	}

	return SDL_FLIP_NONE;
}