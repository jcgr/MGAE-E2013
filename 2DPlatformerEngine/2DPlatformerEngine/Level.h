#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>

#include "Window.h"
#include "Timer.h"
#include "Map.h"
#include "Player.h"

class Level
{
	public:
		Level();
		~Level(){};

		#define GRAVITY 7

		/**
		* The dimensions of the tile.
		*/
		const int TILE_HEIGHT = 64, TILE_WIDTH = 64;

		/**
		* Tells the game to shut down.
		*/
		bool gameShutDown;

		/**
		* Loads the level.
		* @param level The level to load.
		*/
		void load(string level);
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
		* Updates the player.
		*/
		void updatePlayer();
		/**
		* Updates the map.
		*/
		void updateMap();
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
		* Draws the win screen.
		*/
		void drawWinScreen();

		/**
		* Calculates the collision points for the given position.
		* @param newPosX The x coordiate of the new position.
		* @param newPosY The y coordiate of the new position.
		* @param collisionPoints The array to put the results into.
		*/
		void calculateCollisionPoints(int newPosX, int newPosY, SDL_Point* collisionPoints);

		void handleCollision(int collisionType);

		/**
		* Checks if the player is colliding with something in the map.
		* @param newPosX The x-coordinate of the position to check for.
		* @param newPosY The y-coordinate of the position to check for.
		* @return A value that indicates the kind of collision
		*/
		int checkPlayerCollision(int newPosX, int newPosY);

		/**
		* Loads the data from the testMap file, which
		* holds the layout of the map.
		* @return True if the load was successful; false if it was not.
		*/
		bool loadMap();
};

#endif // !LEVEL_H