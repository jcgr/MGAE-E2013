#include "Game.h"

int main(int argc, char* args[])
{
	Game game = Game();

	game.initialize();

	game.gameLoop();

	game.end();

	return 0;
}