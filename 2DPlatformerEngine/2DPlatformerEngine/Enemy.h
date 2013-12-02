#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

#include "Map.h"
#include "HelperClass.h"
#include "Window.h"

/**
* An enemy.
*/
class Enemy
{
public:
	Enemy();
	~Enemy(){};

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
	* The dimensions of the enemy.
	*/
	int enemyHeight, enemyWidth;
	/**
	* The clip counter.
	*/
	int internalClipCounter, currentClip, maxClips;
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

	/**
	* The helper class.
	*/
	HelperClass hc;

	/**
	* A value that tells if the enemy is falling or not.
	*/
	bool isFalling;

	/**
	* The collision points of the enemy.
	*/
	SDL_Point collisionPoints[COLLISION_POINT_AMOUNT];

	/**
	* Moves the enemy.
	* @param map The map (used for collision).
	*/
	virtual void move(Map map);

	/**
	* Loads an enemy.
	* @param x The x-coordinate of the enemy.
	* @param y The y-coordinate of the enemy.
	*/
	virtual void loadEnemy(int x, int y);

	/**
	* Updates of the enemy's texture to reflect what the enemy is doing.
	*/
	virtual void updateTexture();

	/**
	* Updates of the enemy's speed.
	*/
	virtual void updateSpeed();

	/**
	* Gets a value that indicates how the enemy's texture is supposed to be flipped.
	*/
	virtual SDL_RendererFlip getFlip();

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

	/**
	* Checks for collision between the enemy and the map.
	* @param map The map of tiles.
	*/
	int checkCollision(Map map);

protected:

	/**
	* Loads the things an enemy needs.
	* @param x The x-coordinate of the enemy.
	* @param y The y-coordinate of the enemy.
	* @param height The height of the enemy.
	* @param width The width of the enemy.
	* @param maxAnimationClips The amount of clips in the enemy's animation.
	* @param texture The path to the texture.
	*/
	void load(int x, int y, int height, int width, int maxAnimationClips, string texture);
};

#endif