#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

#include "GameVariables.h"

/**
* An enemy.
*/
class Enemy
{
	public:
		Enemy();
		~Enemy(){};

		/**
		* The enemy's type.
		*/
		int enemyType;
		/**
		* The direction the enemy is moving.
		*/
		int moveDirection;
		/**
		* The position of the enemy.
		*/
		int posX, posY;
		/**
		* The enemy's velocity.
		*/
		int velX, velY;

		/**
		* A value that tells if the enemy is falling or not.
		*/
		bool isFalling;

		SDL_Point currentCollisionPoints[COLLISION_POINT_AMOUNT], tempCollisionPoints[COLLISION_POINT_AMOUNT];

		/**
		* Loads an enemy at the given position.
		* @param x The x-coordinate of the enemy.
		* @param y The y-coordinate of the enemy.
		*/
		void loadEnemy(int x, int y, int enemyType);

		/**
		* Updates the harpy's texture to correspond to what the
		* enemy is doing.
		*/
		void updateTexture();

		/**
		* Updates the enemy's speed.
		*/
		void updateSpeed();

		/**
		* Gets the rectangle that take corresponds to the right
		* part of the current animation.
		* @return The rectangle with the right values
		*/
		SDL_Rect getCurrentAnimationClip();

		/**
		* Gets the current texture.
		* @return The current player texture.
		*/
		SDL_Texture* getCurrentTexture();

		int getHeight();
		int getWidth();

	private:

		/**
		* The dimensions of the enemy.
		*/
		int enemyHeight, enemyWidth;
		/**
		* The clip counter.
		*/
		int internalClipCounter, currentClip;
		/**
		* The default velocity.
		*/
		int internalVelX, internalVelY;
		/**
		* The maximum velocity.
		*/
		int maxVelX, maxVelY;

		/**
		* The clips for the texture.
		*/
		SDL_Rect enemyAnimationClips[4];

		/**
		* The texture of the enemy.
		*/
		SDL_Texture *enemyTexture;
};

#endif