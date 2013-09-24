#ifndef GAME_H
#define GAME_H

#include "Level.h"

class Game
{
	public:
		Game();
		~Game(){};

		void initialize();
		void play();
		void end();

	private:

		void showEndScreen();
};

#endif