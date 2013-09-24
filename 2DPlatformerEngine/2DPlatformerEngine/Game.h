#ifndef GAME_H
#define GAME_H

#include "Level.h"

class Game
{
	public:
		Game();
		~Game(){};

		/**
		* Initialize the game.
		*/
		void initialize();
		/**
		* Run the game.
		*/
		void play();
		/**
		* End the game and clean up.
		*/
		void end();

	private:

		/**
		* Show the ending screen.
		*/
		void showEndScreen();
};

#endif