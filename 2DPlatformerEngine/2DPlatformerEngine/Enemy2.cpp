#include "Enemy2.h"
#include "Window.h"

Enemy2::Enemy2()
{
}

void Enemy2::loadEnemy(int x, int y, int height, int width, int maxAnimationClips, string texture)
{
	return;
}

void Enemy2::move(Map map)
{
	return;
}

void Enemy2::updateTexture()
{
	return;
}

void Enemy2::updateSpeed()
{
	// If the Enemy2 is falling, don't move horizontally
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

SDL_Rect Enemy2::getCurrentAnimationClip()
{
	if (isFalling) {
		return enemyAnimationClips[0];
	}

	return enemyAnimationClips[currentClip];
}

SDL_Texture* Enemy2::getCurrentTexture()
{
	return enemyTexture;
}

int Enemy2::getHeight()
{
	return enemyHeight;
}

int Enemy2::getWidth()
{
	return enemyWidth;
}

int Enemy2::checkCollision(Map map)
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