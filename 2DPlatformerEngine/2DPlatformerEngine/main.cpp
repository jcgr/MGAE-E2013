#include "Level.h"

int main(int argc, char* args[])
{
	Level level = Level();

	level.initialize();

	level.gameLoop();

	level.end();

	return 0;
}