#ifndef ENEMYBEAR_H
#define ENEMYBEAR_H

#include "Enemy.h"
#include "HelperClass.h"

class EnemyBear : public Enemy
{
public:
	EnemyBear();
	~EnemyBear(){};

	/**
	* Bear specific movement.
	* @param map The map (used for collision).
	*/
	virtual void move(Map map);

	/**
	* Loads a bear at the given position.
	* @param x The x-coordinate of the bear.
	* @param y The y-coordinate of the bear.
	*/
	virtual void loadEnemy(int x, int y);

	/**
	* Updates the enemy's texture to reflect what the enemy is doing.
	*/
	virtual void updateTexture();

	/**
	* Gets a value that indicates how the bear's texture is supposed to be flipped.
	*/
	virtual SDL_RendererFlip getFlip();
};

#endif