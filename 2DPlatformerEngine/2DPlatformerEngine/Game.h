#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "Window.h"
#include "Timer.h"
#include "Map.h"
#include "Player.h"

class Game
{
	public:
		Game();
		~Game(){};

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

		bool keyDown;

		Player player;
		Map map;

		SDL_Texture* backgroundTexture;
		SDL_Texture* brickTexture;
		SDL_Texture* playerTexture;
		SDL_Texture* testMsg, *keyTest;

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
		* Draws the background.
		*/
		void drawBackground();
		/**
		* Draws the level.
		*/
		void drawLevel();
		/**
		* Draws the player.
		*/
		void drawPlayer();

		/**
		* Loads the data from the testMap file, which
		* holds the layout of the map.
		* @return True if the load was successful; false if it was not.
		*/
		bool loadMap();
};

#endif // !GAME_H