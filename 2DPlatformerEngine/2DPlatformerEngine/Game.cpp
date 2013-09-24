#include <SDL_image.h>

#include "Game.h"

using namespace std;

Game::Game()
{
	map = Map(TILE_HEIGHT, TILE_WIDTH);
	player = Player(1080, 500);
}

void Game::initialize()
{
	Window::Init("2D Platformer");

	backgroundPosition = {
		0, 0, Window::Box().w, Window::Box().h
	};

	backgroundTexture = Window::LoadImage("media/background.jpg");
	brickTexture = Window::LoadImage("media/brick64px.png");
	goalTexture = Window::LoadImage("media/Goal64px.png");

	player.loadPlayer();

	map.loadMap("testMap.txt");
}

void Game::gameLoop()
{
	//Our event structure
	SDL_Event e;

	//For tracking if we want to quit
	bool quit = false;
	while (!quit)
	{
		bool keyPressed = false;

		previousKey1 = currentKey1;
		previousKey2 = currentKey2;

		//// Event Polling
		while (SDL_PollEvent(&e))
		{
			// Update keyboardState
			SDL_PumpEvents();

			// If user closes the window
			if (e.type == SDL_QUIT){
				quit = true;
			}

			currentKey1 = 0;
			currentKey2 = 0;

			if (keyboardState[SDL_SCANCODE_D]){
				player.accelerateX();
				player.moveState = MOVE_RIGHT;
				currentKey1 = SDLK_d;
			}
			if (keyboardState[SDL_SCANCODE_A]){
				player.accelerateX();
				player.moveState = MOVE_LEFT;
				currentKey1 = SDLK_a;
			}
			if (keyboardState[SDL_SCANCODE_W]){
				player.accelerateY();
				currentKey2 = SDLK_w;
			}
			if (keyboardState[SDL_SCANCODE_ESCAPE]){
				quit = true;
			}
		}

		updateMap();
		updatePlayer();
		updateEnemies();

		Window::Clear();
		Window::Draw(backgroundTexture, backgroundPosition);
		drawLevel();
		drawPlayer();
		Window::Present();
	}
}

void Game::end()
{
	Window::Clear();
	Window::Quit();
}

void Game::updatePlayer()
{
	// Check if the player needs to lose speed
	// on any of the axis.
	if (currentKey1 == 0){
		player.decelerateX();
	}
	if (currentKey2 == 0){
		player.decelerateY();
	}

	player.move(map);
}

void Game::updateMap()
{

}

void Game::updateEnemies()
{

}

void Game::drawLevel()
{
	int** tempMap = map.getMap();
	int firstX, camPosX, offsetX;
	int height = map.getHeight(), width = map.getWidth();

	// Calculate which part of the level to show, based on the player's
	// position. Make sure it does not exceed the bounds of the map.
	camPosX = (player.getPosX()) - (Window::WINDOW_WIDTH / 2);
	if (camPosX < 0) {
		camPosX = 0;
	}
	if (camPosX + Window::WINDOW_WIDTH > width * TILE_WIDTH) {
		camPosX = (map.getWidth() * TILE_WIDTH) - Window::WINDOW_WIDTH;
	}

	// The first tile on the x-axis and how much of it that can
	// be seen.
	firstX = camPosX / TILE_WIDTH;
	offsetX = camPosX % TILE_WIDTH;

	// Draw the level.
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++) {

			// Find the actual tile in the tilemap
			// we're working with based on the firstX.
			int currentX;
			if (x + firstX < width){
				currentX = x + firstX;
			}
			else {
				currentX = width - 1;
			}

			// Draw the tiles.
			if (tempMap[y][currentX] == 1)
			{
				int tempWidth, tempHeight;
				SDL_QueryTexture(brickTexture, NULL, NULL, &tempWidth, &tempHeight);

				SDL_Rect pos;
				pos.x = (x * TILE_WIDTH) - (offsetX);
				pos.y = y * TILE_HEIGHT;
				pos.w = tempWidth;
				pos.h = tempHeight;

				Window::Draw(brickTexture, pos);
			}

			// Draw the goal.
			if (tempMap[y][currentX] == 2)
			{
				int tempWidth, tempHeight;
				SDL_QueryTexture(goalTexture, NULL, NULL, &tempWidth, &tempHeight);

				SDL_Rect pos;
				pos.x = (x * TILE_WIDTH) - (offsetX) - ((tempWidth - TILE_WIDTH) / 2);
				pos.y = y * TILE_HEIGHT - (tempHeight - TILE_HEIGHT);
				pos.w = tempWidth;
				pos.h = tempHeight;

				Window::Draw(goalTexture, pos);
			}
		}
	}
}

void Game::drawPlayer()
{
	// these are used to determine if the player is close
	// to one end of the screen.
	int left, right;
	left = Window::WINDOW_WIDTH / 2;
	right = map.getWidth() * 64 - (left);

	// Get player position. Adjust to take height/width
	// of the character into account.
	SDL_Rect pos;
	pos.x = player.getPosX() - (player.playerWidth / 2);
	pos.y = player.getPosY() - (player.playerHeight / 2);
	pos.w = player.playerWidth;
	pos.h = player.playerHeight;

	// If the player is not close to any side...
	if (player.getPosX() > left && player.getPosX() < right){
		// ... set his x position to the middle of the screen.
		pos.x = Window::WINDOW_WIDTH / 2
			- (player.playerWidth / 2);
	}
	// If the player is close to the right side of the level...
	if (player.getPosX() >= right) {
		// ... set his x position to show how close he is.
		pos.x = Window::WINDOW_WIDTH / 2
			+ (player.getPosX() - right)
			- (player.playerWidth / 2);
	}

	// Draw the player
	Window::Draw(player.getCurrentTexture(), pos, &player.getCurrentAnimationClip());
}