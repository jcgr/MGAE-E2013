#include <fstream>

#include "Game.h"

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
	Level level = Level();

	int levelNumber = 0;
	string levelFile = "Level";
	string levelFileExtention = ".txt";
	bool gameOver = false, showVictoryScreen = false;

	while (!gameOver)
	{
		string levelFileName = levelFile + std::to_string(levelNumber) + levelFileExtention;
		ifstream mapData(levelFileName.c_str());

		if (mapData.is_open())
		{
			level.load(levelFileName);

			level.run();

			level.end();

			levelNumber++;
		}

		if (!mapData.is_open())
		{
			gameOver = true;
			showVictoryScreen = true;
		}
	}

	// Shows the victory screen after the level
	while (showVictoryScreen)
	{
		//Our event structure
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
		showEndScreen();
		Window::Present();
	}
}

void Game::end()
{
	Window::Clear();
	Window::Quit();
}

// Private

void Game::showEndScreen()
{
	SDL_Color white = { 255, 255, 255 };
	SDL_Texture *msgGrats, *msgContinue;
	SDL_Rect msgGratsBox, msgContinueBox;

	msgGrats = Window::RenderText("No more levels to load! You have won!", "FreeSans.ttf", white, 50);
	msgContinue = Window::RenderText("Press [escape] to stop the game.", "FreeSans.ttf", white, 30);

	SDL_QueryTexture(msgGrats, NULL, NULL, &msgGratsBox.w, &msgGratsBox.h);
	SDL_QueryTexture(msgContinue, NULL, NULL, &msgContinueBox.w, &msgContinueBox.h);

	msgGratsBox.x = (Window::Box().w / 2) - (msgGratsBox.w / 2);
	msgGratsBox.y = (Window::Box().h / 2) - (msgGratsBox.h / 2) - 25;

	msgContinueBox.x = (Window::Box().w / 2) - (msgContinueBox.w / 2);
	msgContinueBox.y = (Window::Box().h / 2) - (msgContinueBox.h / 2) + 25;

	Window::Draw(msgGrats, msgGratsBox);
	Window::Draw(msgContinue, msgContinueBox);
}