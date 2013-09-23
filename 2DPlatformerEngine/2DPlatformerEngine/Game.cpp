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

	player.playerStandRight = Window::LoadImage("media/SamusStandRight64px.png");
	player.playerStandLeft = Window::LoadImage("media/SamusStandLeft64px.png");
	player.playerWalkRight = Window::LoadImage("media/SamusWalkRight64px.png");
	player.playerWalkLeft = Window::LoadImage("media/SamusWalkLeft64px.png");
	player.playerJumpRight = Window::LoadImage("media/SamusJumpRight64px.png");
	player.playerJumpLeft = Window::LoadImage("media/SamusJumpLeft64px.png");

	player.currentTexture = player.playerStandRight;
	player.loadPlayer(player.playerStandRight);

	map.loadMap("testMap.txt");

	keyDown = false;
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
		drawBackground();
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
	player.move(map);

	// Check if the player needs to lose speed
	// on any of the axis.
	if (currentKey1 == 0){
		player.decelerateX();
	}
	if (currentKey2 == 0){
		player.decelerateY();
	}
}

void Game::updateMap()
{

}

void Game::updateEnemies()
{

}

void Game::drawBackground()
{
	Window::Draw(backgroundTexture, backgroundPosition);
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

			// Draw the tile.
			if (tempMap[y][currentX] == 1)
			{
				SDL_Rect pos;
				pos.x = (x * TILE_WIDTH) - (offsetX);
				pos.y = y * TILE_HEIGHT;
				pos.w = TILE_WIDTH;
				pos.h = TILE_HEIGHT;

				Window::Draw(brickTexture, pos);
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

	// Draw the right texture (this part looks like shit D:)
	if (player.isJumping){
		SDL_Rect* clips = player.getAnimationJumpClips();
		Window::Draw(player.currentTexture, pos, &clips[player.currentJumpClip]);
	}
	else if (player.moveState == MOVE_LEFT || player.moveState == MOVE_RIGHT) {
		SDL_Rect* clips = player.getAnimationWalkClips();
		Window::Draw(player.currentTexture, pos, &clips[player.currentWalkClip]);
	}
	else {
		Window::Draw(player.currentTexture, pos);
	}
}