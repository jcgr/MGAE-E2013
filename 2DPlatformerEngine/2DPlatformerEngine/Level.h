#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <list>

#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"

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
		* The x-coordinate of the camera (used for drawing enemies and level)
		*/
		int camPosX;

		int numberOfEnemies;

		/**
		* A list of enemies in the level.
		*/
		Enemy* enemyList;

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
		* Logic for moving harpies.
		* @param harpy The initial information about the harpy.
		* @return The moved harpy.
		*/
		Enemy moveHarpy(Enemy harpy);
		/**
		* Logic for moving grizzlies.
		* @param grizzly The initial information about the grizzly.
		* @return The moved grizzly.
		*/
		Enemy moveGrizzly(Enemy grizzly);

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

		/**
		* Calculates the collision points for the given position.
		* @param newPosX The x coordiate of the new position.
		* @param newPosY The y coordiate of the new position.
		* @param collisionPoints The array to put the results into.
		*/
		void calculateCollisionPoints(int height, int width, int newPosX, int newPosY, SDL_Point* collisionPoints);

		/**
		* Makes stuff happen to the player, depending on what he collides with
		* @param collisionType The type of collision that happens.
		*/
		void handlePlayerCollision(int collisionType);

		/**
		* Checks if the points collide with something in the map.
		* @param *collisoinPoints The points to do the collision checking for.
		* @param player A value that determines if we're checking for the player or not.
		* @return A value that indicates the kind of collision.
		*/
		int checkCollision(SDL_Point *collisionPoints, bool player);
};

#endif // !LEVEL_H