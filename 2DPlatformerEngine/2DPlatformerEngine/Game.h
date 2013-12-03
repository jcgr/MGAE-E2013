#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "Player.h"

/**
* The Game class is used to demonstrate one way of using the engine.
*/
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
};

#endif