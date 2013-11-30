#ifndef ENEMY2BEAR_H
#define ENEMY2BEAR_H

#include "Enemy2.h"
#include "HelperClass.h"

class Enemy2Bear : public Enemy2
{
public:
	Enemy2Bear();
	~Enemy2Bear(){};

	/**
	* Moves the enemy.
	* @param map The map (used for collision).
	*/
	virtual void move(Map map);

	/**
	* Loads an enemy at the given position.
	* @param x The x-coordinate of the enemy.
	* @param y The y-coordinate of the enemy.
	* @param height The height of the enemy.
	* @param width The width of the enemy.
	* @param maxAnimationClips The amount of clips in the enemy's animation.
	* @param texture The path to the texture.
	*/
	virtual void loadEnemy(int x, int y, int height, int width, int maxAnimationClips, string texture);

	/**
	* Updates the enemy's texture to reflect what the enemy is doing.
	*/
	virtual void updateTexture();
};

#endif