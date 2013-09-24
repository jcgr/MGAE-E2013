#include <iostream>
#include <fstream>
#include <string>

#include "Map.h"

using namespace std;

Map::Map()
{
	Map::tileHeight = 64;
	Map::tileWidth = 64;
}

Map::Map(int tileHeight, int tileWidth)
{
	Map::tileHeight = tileHeight;
	Map::tileWidth = tileWidth;
}

void Map::loadMap(string fileName)
{
	string line;

	// Open map file
	ifstream mapData(fileName.c_str());

	if (mapData.is_open())
	{
		// Read height of the map
		getline(mapData, line);
		Map::height = atoi(line.c_str());

		// Read width of the map
		getline(mapData, line);
		Map::width = atoi(line.c_str());

		tileMap = new int*[Map::width];

		for (int row = 0; row < Map::height; ++row){
			tileMap[row] = new int[Map::width];
		}
		for (int row = 0; row < Map::height; ++row){
			for (int col = 0; col < Map::width; ++col){
					tileMap[row][col] = 0;
			}
		}

		// Read each line of the map file and fill in the
		// integers in the gameMap array.
		for (int row = 0; row < Map::height; row++)
		{
			getline(mapData, line);
			char mapTiles[1000];
			strcpy_s(mapTiles, line.c_str());

			for (int col = 0; col < Map::width; col++)
			{
				// -48 'cause char
				tileMap[row][col] = mapTiles[col] - 48;
			}
		}
	}
	else
	{
		cerr << SDL_GetError() << endl;
	}
}

int** Map::getMap()
{
	return tileMap;
}

int Map::getTile(int x, int y)
{
	// Checks if the pixel is within the map.
	if (x > width * tileWidth || x < 0)
		return TILE_SOLID_BLOCK;
	if (y > height * tileHeight || y < 0)
		return TILE_SOLID_BLOCK;

	// Finds the tile location of the pixel.
	// Remove tileWidth/Height / 2 because of how the window
	// moves everything it draws (the given position becomes the
	// center instead of the top-right point).
	int tileX = (x - (tileWidth / 2)) / tileWidth;
	int tileY = (y - (tileHeight / 2)) / tileHeight;

	if (tileMap[tileY][tileX] == 1)
		return TILE_SOLID_BLOCK;
	if (tileMap[tileY][tileX] == 2 || tileMap[tileY][tileX] == 3)
		return TILE_GOAL;

	return TILE_EMPTY;
}

SDL_Rect Map::getPlayerSpawn()
{
	SDL_Rect spawn;
	spawn.x = 300;
	spawn.y = 300;

	for (int row = 0; row < Map::height; row++) {
		for (int col = 0; col < Map::width; col++) {
			if (tileMap[row][col] == 9) {
				spawn.y = row * Map::tileHeight + 20;
				spawn.x = col * Map::tileWidth;
			}
		}
	}

	return spawn;
}

int Map::getHeight()
{
	return height;
}

int Map::getWidth()
{
	return width;
}