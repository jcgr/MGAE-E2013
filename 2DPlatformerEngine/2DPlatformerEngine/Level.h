#ifndef GAME_H
#define GAME_H

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

		const int TILE_HEIGHT = 64;
		const int TILE_WIDTH = 64;

		/**
		* Initializes the game.
		*/
		void initialize();
		/**
		* A loop in the game.
		*/
		void gameLoop();
		/**
		* Ends the game and cleans stuff up.
		*/
		void end();

	private:

		int previousKey1, previousKey2;
		int currentKey1, currentKey2;

		Player player;
		Map map;

		SDL_Texture *backgroundTexture;
		SDL_Texture *brickTexture, *goalTexture;
		SDL_Texture *testMsg, *keyTest;

		const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

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
		int checkCollision(int newPosX, int newPosY);

		/**
		* Loads the data from the testMap file, which
		* holds the layout of the map.
		* @return True if the load was successful; false if it was not.
		*/
		bool loadMap();
};

#endif // !GAME_H