#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <vector>

using namespace std;

/**
* A class that represents the level in the game.
*/
class Map
{
	public:
		Map();
		Map(int tileHeight, int tileWidth);
		~Map(){};

		#define TILE_EMPTY 0
		#define TILE_SOLID_BLOCK 1
		#define TILE_GOAL 2
		#define TILE_PLAYER_SPAWN 9

		/**
		* Loads a grid based map from the given file.
		* @param &fileName The name of the file that contains the map.
		*/
		void loadMap(string fileName);

		/**
		* Gets the tile map.
		* @return The map as a 2D array of integers.
		*/
		int** getMap();

		/**
		* Gets the type of tile at the given position
		* @param x The x coordinate of the pixel.
		* @param y The y coordinate of the pixel.
		* @return The type of tile at the position.
		*/
		int getTile(int x, int y);

		/**
		* Gets the player's spawn location.
		* @return A SDL_Rect with the spawn position.
		*/
		SDL_Rect getPlayerSpawn();

		/**
		* Gets the height of the map in tiles.
		* @return The height of the map in tiles.
		*/
		int getHeight();
		/**
		* Gets the width of the map in tiles.
		* @return The width of the map in tiles.
		*/
		int getWidth();

	private:

		int** tileMap;
		int height, width;
		int tileHeight, tileWidth;

};

#endif // !MAP_H