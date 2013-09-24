#include "Game.h"
//#include "Level.h"

int main(int argc, char* args[])
{
	//Level level = Level();

	//level.initialize("testMap.txt");

	//level.gameLoop();

	//level.end();

	Game game = Game();
	game.initialize();
	game.play();
	game.end();

	return 0;
}