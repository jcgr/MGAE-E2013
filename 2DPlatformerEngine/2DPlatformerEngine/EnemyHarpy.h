#ifndef ENEMYHARPY_H
#define ENEMYHARPY_H

#include "Enemy.h"
#include "HelperClass.h"

class EnemyHarpy : public Enemy
{
public:
	EnemyHarpy();
	~EnemyHarpy(){};

	/**
	* Harpy specific movement.
	* @param map The map (used for collision).
	*/
	virtual void move(Map map);

	/**
	* Loads a harpy at the given position.
	* @param x The x-coordinate of the harpy.
	* @param y The y-coordinate of the harpy.
	*/
	virtual void loadEnemy(int x, int y);

	/**
	* Gets a value that indicates how the harpy's texture is supposed to be flipped.
	*/
	virtual SDL_RendererFlip getFlip();
};

#endif