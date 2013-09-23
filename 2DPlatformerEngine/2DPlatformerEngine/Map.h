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
		* Checks if the given pixel contains a solid tile.
		* @param x The x coordinate of the pixel.
		* @param y The y coordinate of the pixel.
		* @return True if the pixel is part of a solid tile; false if it is not.
		*/
		bool isPixelSolid(int x, int y);

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