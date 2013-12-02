#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <list>

#include "HelperClass.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBear.h"
#include "EnemyHarpy.h"

class Level
{
public:
	Level();
	~Level(){};

	/**
	* Tells the game to shut down.
	*/
	bool gameShutDown;

	/**
	* Loads the level.
	* @param level The level to load.
	* @param playerCharacter The player character.
	*/
	void load(string level, Player playerCharacter);
	/**
	* Runs the game.
	*/
	void run();

private:

	/**
	* Determines which key is currently pressed.
	*/
	int currentHorizontalKey, currentVerticalKey;

	/**
	* The x-coordinate of the camera (used for drawing enemies and level)
	*/
	int camPosX;

	/**
	* The actual number of enemies in the level.
	*/
	int numberOfEnemies;

	/**
	* A list of enemies in the level.
	*/
	Enemy *enemy2List[MAX_AMOUNT_OF_EMENIES];

	/**
	* The player object.
	*/
	Player player;
	/**
	* The map object.
	*/
	Map map;

	/**
	* The texture of the background.
	*/
	SDL_Texture *backgroundTexture;
	/**
	* The texture of game objects.
	*/
	SDL_Texture *brickTexture, *goalTexture, *spikeTexture;

	/**
	* The state of the keyboard.
	*/
	const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

	/**
	* The position and dimension of the background.
	*/
	SDL_Rect backgroundPosition;

	/**
	* The helper class
	*/
	HelperClass hc;

	/**
	* Updates the player.
	*/
	void updatePlayer();
	/**
	* Updates the enemies.
	*/
	void updateEnemies();

	/**
	* Moves the player.
	*/
	void movePlayer();

	/**
	* Draws the level.
	*/
	void drawLevel();
	/**
	* Draws the player.
	*/
	void drawPlayer();
	/**
	* Draws the enemies.
	*/
	void drawEnemies();
	/**
	* Draws the win screen.
	*/
	void drawWinScreen();

	/**
	* Loads the enemies present on the map.
	*/
	void loadEnemiesFromMap();
};

#endif // !LEVEL_H