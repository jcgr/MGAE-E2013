#include <SDL_image.h>

#include "Level.h"

// PUBLIC
Level::Level()
{
}

void Level::load(string level)
{
	// The position/size of the background.
	backgroundPosition = {
		0, 0, Window::Box().w, Window::Box().h
	};

	// Loads the map and gets the player spawn.
	map = Map(TILE_HEIGHT, TILE_WIDTH);
	map.loadMap(level);
	SDL_Rect initialSpawn = map.getPlayerSpawn();

	// Loads the player.
	player = Player();
	player.setSpawn(initialSpawn.x, initialSpawn.y);
	player.loadPlayer();
	calculateCollisionPoints(player.posX, player.posY, player.currentCollisionPoints);

	backgroundTexture = Window::LoadImage("media/background.jpg");
	brickTexture = Window::LoadImage("media/brick64px.png");
	goalTexture = Window::LoadImage("media/Goal64px.png");
	spikeTexture = Window::LoadImage("media/Spikes64px.png");
}

void Level::run()
{
	SDL_Event e;

	//For tracking if we want to quit
	bool quit = false;
	bool showVictoryScreen = false;
	gameShutDown = false;

	// Main loop for level
	while (!quit)
	{
		// Event Polling
		while (SDL_PollEvent(&e))
		{
			// Update keyboardState
			SDL_PumpEvents();

			// If user closes the window
			if (e.type == SDL_QUIT){
				quit = true;
				gameShutDown = true;
			}

			currentHorizontalKey = 0;
			currentVerticalKey = 0;

			if (keyboardState[SDL_SCANCODE_D]){
				player.accelerateX();
				player.moveState = MOVE_RIGHT;
				currentHorizontalKey = SDLK_d;
			}
			if (keyboardState[SDL_SCANCODE_A]){
				player.accelerateX();
				player.moveState = MOVE_LEFT;
				currentHorizontalKey = SDLK_a;
			}
			if (keyboardState[SDL_SCANCODE_W]){
				player.accelerateY();
				currentVerticalKey = SDLK_w;
			}
			if (keyboardState[SDL_SCANCODE_ESCAPE]){
				quit = true;
				gameShutDown = true;
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

		if (player.reachedGoal) {
			quit = true;
			showVictoryScreen = true;
		}
	}

	// Shows the victory screen after the level
	while (showVictoryScreen)
	{
		while (SDL_PollEvent(&e))
		{
			// Update keyboardState
			SDL_PumpEvents();

			// If user closes the window
			if (e.type == SDL_QUIT) {
				showVictoryScreen = false;
			}

			if (keyboardState[SDL_SCANCODE_SPACE]) {
				showVictoryScreen = false;
			}
		}

		Window::Clear();
		drawWinScreen();
		Window::Present();
	}
}

void Level::updatePlayer()
{
	// Check if the player needs to lose speed
	// on any of the axis.
	if (currentHorizontalKey == 0){
		player.decelerateX();
	}
	if (currentVerticalKey == 0){
		player.decelerateY();
	}

	movePlayer();
	player.updateTexture();
	calculateCollisionPoints(player.posX, player.posY, player.currentCollisionPoints);

	if (player.health <= 0) {
		player.respawn();
	}
}

void Level::updateMap()
{
	// Does nothing
}

void Level::updateEnemies()
{
	// Does nothing
}

void Level::movePlayer()
{
	// If the player if moving left ...
	if (player.moveState == MOVE_LEFT) {
		for (int i = 0; i < player.velX; i++)
		{
			int collisionType = checkCollision(player.posX - 1, player.posY);
			if (collisionType) {
				handleCollision(collisionType);
				break;
			}

			player.posX--;
		}
	}

	// If the player is moving right ...
	if (player.moveState == MOVE_RIGHT) {
		for (int i = 0; i < player.velX; i++)
		{
			int collisionType = checkCollision(player.posX + 1, player.posY);
			if (collisionType) {
				handleCollision(collisionType);
				break;
			}

			player.posX++;
		}
	}

	// If the player is moving up ...
	if (player.velY > 0) {
		// ... check each position step-by-step instead of just moving
		// the player 7 pixels immediately.
		for (int i = 0; i < GRAVITY; i++)
		{
			int collisionType = checkCollision(player.posX, player.posY - 1);
			if (collisionType) {
				handleCollision(collisionType);
				player.decelerateY();
				break;
			}

			player.posY--;
			player.velY--;
		}
	}
	// If the player is moving down ...
	else {
		for (int i = player.velY; i < 0; i++)
		{
			// If the player collides with something below, he has
			// landed and can jump again.
			int collisionType = checkCollision(player.posX, player.posY + 1);
			if (collisionType) {
				handleCollision(collisionType);
				player.canJump = true;
				player.isJumping = false;
				break;
			}

			player.posY++;
		}
	}
}

void Level::drawLevel()
{
	int** tempMap = map.getMap();
	int firstX, camPosX, offsetX;
	int height = map.getHeight(), width = map.getWidth();

	// Calculate which part of the level to show, based on the player's
	// position. Make sure it does not exceed the bounds of the map.
	camPosX = (player.posX) - (Window::WINDOW_WIDTH / 2);
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

			// Draw the spikes.
			if (tempMap[y][currentX] == 4)
			{
				int tempWidth, tempHeight;
				SDL_QueryTexture(brickTexture, NULL, NULL, &tempWidth, &tempHeight);

				SDL_Rect pos;
				pos.x = (x * TILE_WIDTH) - (offsetX);
				pos.y = y * TILE_HEIGHT;
				pos.w = tempWidth;
				pos.h = tempHeight;

				Window::Draw(spikeTexture, pos);
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

void Level::drawPlayer()
{
	// these are used to determine if the player is close
	// to one end of the screen.
	int left, right;
	left = Window::WINDOW_WIDTH / 2;
	right = map.getWidth() * 64 - (left);

	// Get player position. Adjust to take height/width
	// of the character into account.
	SDL_Rect pos;
	pos.x = player.posX - (player.getWidth() / 2);
	pos.y = player.posY - (player.getHeight() / 2);
	pos.w = player.getWidth();
	pos.h = player.getHeight();

	// If the player is not close to any side...
	if (player.posX > left && player.posX < right){
		// ... set his x position to the middle of the screen.
		pos.x = Window::WINDOW_WIDTH / 2
			- (player.getWidth() / 2);
	}
	// If the player is close to the right side of the level...
	if (player.posX >= right) {
		// ... set his x position to show how close he is.
		pos.x = Window::WINDOW_WIDTH / 2
			+ (player.posX - right)
			- (player.getWidth() / 2);
	}

	// Draw the player
	Window::Draw(player.getCurrentTexture(), pos, &player.getCurrentAnimationClip());
}

void Level::drawWinScreen()
{
	SDL_Color white = { 255, 255, 255 };
	SDL_Texture *msgGrats, *msgContinue;
	SDL_Rect msgGratsBox, msgContinueBox;

	msgGrats = Window::RenderText("You have completed the level!", "FreeSans.ttf", white, 50);
	msgContinue = Window::RenderText("Press [space] to continue.", "FreeSans.ttf", white, 30);

	SDL_QueryTexture(msgGrats, NULL, NULL, &msgGratsBox.w, &msgGratsBox.h);
	SDL_QueryTexture(msgContinue, NULL, NULL, &msgContinueBox.w, &msgContinueBox.h);

	msgGratsBox.x = (Window::Box().w / 2) - (msgGratsBox.w / 2);
	msgGratsBox.y = (Window::Box().h / 2) - (msgGratsBox.h / 2) - 25;

	msgContinueBox.x = (Window::Box().w / 2) - (msgContinueBox.w / 2);
	msgContinueBox.y = (Window::Box().h / 2) - (msgContinueBox.h / 2) + 25;

	Window::Draw(msgGrats, msgGratsBox);
	Window::Draw(msgContinue, msgContinueBox);
}

int Level::checkCollision(int newPosX, int newPosY)
{
	int tileType = 0;

	// Get temp collision points
	calculateCollisionPoints(newPosX, newPosY, player.tempCollisionPoints);

	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
			SDL_Point tempPoint = player.tempCollisionPoints[i];
			int tempCollisionType = map.getTile(tempPoint.x, tempPoint.y);
			if (tempCollisionType > tileType) {
				tileType = tempCollisionType;
			}
	}

	return tileType;
}

void Level::calculateCollisionPoints(int newPosX, int newPosY, SDL_Point* collisionPoints)
{
	// Calculate 9 collision points. Each corner, midways between
	// each corner and the middle of the texture.
	for (int i = 0; i < 9; i++)
	{
		SDL_Point point;
		point.x = newPosX + ((i / 3) * (player.getWidth() / 2));
		point.y = newPosY + ((i % 3) * (player.getHeight() / 2));

		collisionPoints[i] = point;
	}
}

void Level::handleCollision(int collisionType)
{
	switch (collisionType)
	{
	case TILE_SOLID_BLOCK:
		// Do nothing. Is handled depending on where it happens
		break;

	case TILE_GOAL:
		player.reachedGoal = true;
		break;

	case TILE_SPIKE:
		player.health = 0;
		break;
	}
}