#include <fstream>

#include "Game.h"

/**
* The Game class is used to demonstrate one way of using the engine.
*/

// Public
Game::Game()
{

}

void Game::initialize()
{
	Window::Init("2D Platformer");
}

void Game::play()
{
	Player player = Player();
	Level level = Level();
	HelperClass hc = HelperClass();

	// The file name for the levels.
	int levelNumber = 0;
	string levelFile = "Level";
	string levelFileExtention = ".txt";
	
	bool gameOver = false, showVictoryScreen = false;

	while (!gameOver)
	{
		// Append the level-things to find the next level.
		string levelFileName = levelFile + std::to_string(levelNumber) + levelFileExtention;
		ifstream mapData(levelFileName.c_str());

		// If the file exists ...
		if (mapData.is_open())
		{
			// ... load the level and run it
			level.load(levelFileName, player);
			level.run();

			levelNumber++;
		}

		// If the file does not exist ...
		if (!mapData.is_open())
		{
			// ... end the game
			gameOver = true;
			showVictoryScreen = true;
		}

		if (level.gameShutDown) {
			gameOver = true;
		}
	}

	// Shows the victory screen after the level
	while (showVictoryScreen)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			// If user closes the window
			if (e.type == SDL_QUIT) {
				showVictoryScreen = false;
			}

			if (e.type = SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					showVictoryScreen = false;
				}
			}
		}

		Window::Clear();
		hc.drawGameWonScreen();
		Window::Present();
	}
}

void Game::end()
{
	Window::Clear();
	Window::Quit();
}