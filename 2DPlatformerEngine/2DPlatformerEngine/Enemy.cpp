#include "Enemy.h"
#include "Window.h"

Enemy::Enemy()
{
}

void Enemy::loadEnemy(int x, int y, int height, int width, int maxAnimationClips, string texture)
{
	return;
}

void Enemy::move(Map map)
{
	return;
}

void Enemy::updateTexture()
{
	return;
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