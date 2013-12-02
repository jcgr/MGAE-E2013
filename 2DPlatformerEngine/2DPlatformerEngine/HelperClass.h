#ifndef HELPERCLASS_H
#define HELPERCLASS_H

#include <SDL.h>

#include "Map.h"

class HelperClass
{
	// Level related
	#define LEVEL_TEXTURE_BACKGROUND "media/background.jpg"
	#define LEVEL_TEXTURE_BRICK "media/brick64px.png"
	#define LEVEL_TEXTURE_GOAL "media/Goal64px.png"
	#define LEVEL_TEXTURE_SPIKES "media/Spikes64px.png"

	#define MAX_AMOUNT_OF_EMENIES 100

	// Map related
	#define TILE_HEIGHT 64;
	#define TILE_WIDTH 64;

	#define COLLISION_POINT_AMOUNT 9

	#define COLLISION_EMPTY 0
	#define COLLISION_SOLID_BLOCK 1
	#define COLLISION_GOAL 2
	#define COLLISION_SPIKE 4
	#define COLLISION_ENEMY 5

	// Player related
	#define PLAYER_STAND_RIGHT 1
	#define PLAYER_STAND_LEFT -1
	#define PLAYER_MOVE_RIGHT 2
	#define PLAYER_MOVE_LEFT -2

	#define PLAYER_GRAVITY 7

	#define PLAYER_TEXTURE_STAND "media/SamusStandRight64px.png"
	#define PLAYER_TEXTURE_WALK "media/SamusWalkRight64px.png"
	#define PLAYER_TEXTURE_JUMP "media/SamusJumpRight64px.png"
	#define PLAYER_TEXTURE_DIE "media/SamusDie64px.png"

	#define PLAYER_STAND_CLIPS 1
	#define PLAYER_WALK_CLIPS 10
	#define PLAYER_JUMP_CLIPS 8
	#define PLAYER_DIE_CLIPS 15

	// Enemy related
	#define ENEMY_FLYING 5
	#define ENEMY_WALKING 6

	#define ENEMY_MOVE_UP 1
	#define ENEMY_MOVE_DOWN 2
	#define ENEMY_MOVE_RIGHT 3
	#define ENEMY_MOVE_LEFT 4

	#define ENEMY_TEXTURE_FLYER "media/EnemyHarpy64px.png"
	#define ENEMY_TEXTURE_WALKER "media/EnemyGrizzly64px.png"

	#define ENEMY_WALK_CLIPS 4
	#define ENEMY_FLY_CLIPS 4

	#define ENEMY_FLYER_HEIGHT 64
	#define ENEMY_FLYER_WIDTH 64

	#define ENEMY_WALKER_HEIGHT 64
	#define ENEMY_WALKER_WIDTH 36

public:
	void calculateCollisionPoints(int unitHeight, int unitWidth, int newPosX, int newPosY, SDL_Point* collisionPoints);
};

#endif