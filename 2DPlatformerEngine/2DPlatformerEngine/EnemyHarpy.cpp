#include "EnemyHarpy.h"

EnemyHarpy::EnemyHarpy()
{
}

void EnemyHarpy::loadEnemy(int x, int y)
{
	load(x, y, ENEMY_FLYER_HEIGHT, ENEMY_FLYER_WIDTH, ENEMY_FLY_CLIPS, ENEMY_TEXTURE_FLYER);

	moveDirection = ENEMY_MOVE_UP;

	maxVelX = 0;
	maxVelY = 50;

	isFalling = false;
}

void EnemyHarpy::move(Map map)
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

SDL_RendererFlip EnemyHarpy::getFlip()
{
	return SDL_FLIP_HORIZONTAL;
}