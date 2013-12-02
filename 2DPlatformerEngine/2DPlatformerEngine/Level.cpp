#include <SDL_image.h>

#include "Level.h"

// PUBLIC
Level::Level()
{
}

void Level::load(string level, Player playerCharacter)
{
	// The position/size of the background.
	backgroundPosition = {
		0, 0, Window::Box().w, Window::Box().h
	};

	// Loads the map and gets the player spawn.
	map = Map();
	map.loadMap(level);
	SDL_Rect initialSpawn = map.getPlayerSpawn();

	// Loads the player.
	player = Player();
	player.setSpawn(initialSpawn.x, initialSpawn.y);
	player.loadPlayer();
	hc.calculateCollisionPoints(player.getHeight(), player.getWidth(), player.posX, player.posY, player.collisionPoints);

	loadEnemiesFromMap();

	backgroundTexture = Window::LoadImage(LEVEL_TEXTURE_BACKGROUND);
	brickTexture = Window::LoadImage(LEVEL_TEXTURE_BRICK);
	goalTexture = Window::LoadImage(LEVEL_TEXTURE_GOAL);
	spikeTexture = Window::LoadImage(LEVEL_TEXTURE_SPIKES);
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
		if (player.timeToRespawn) {
			player.respawn();
			loadEnemiesFromMap();
		}

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

			// Using keyboardState as it handles multiple input at once.
			if (keyboardState[SDL_SCANCODE_D]){
				player.accelerateX();
				player.moveState = PLAYER_MOVE_RIGHT;
				currentHorizontalKey = SDLK_d;
			}
			if (keyboardState[SDL_SCANCODE_A]){
				player.accelerateX();
				player.moveState = PLAYER_MOVE_LEFT;
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

		updatePlayer();
		updateEnemies();

		Window::Clear();
		Window::Draw(backgroundTexture, backgroundPosition);
		drawLevel();
		drawPlayer();
		drawEnemies();
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
				gameShutDown = true;
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

	player.updateTexture();
	player.move(map, enemy2List, numberOfEnemies);
}

void Level::updateEnemies()
{
	// If the player is dead, don't do anything
	if (!player.isAlive) {
		return;
	}

	// Update each enemy's speed and texture
	for (int enemy = 0; enemy < numberOfEnemies; enemy++)
	{
		Enemy *tempEnemy = enemy2List[enemy];
		tempEnemy->updateTexture();
		tempEnemy->updateSpeed();
		tempEnemy->move(map);
	}
}

void Level::drawLevel()
{
	int** tempMap = map.getMap();
	int firstX, offsetX;
	int height = map.getHeight(), width = map.getWidth();

	// Calculate which part of the level to show, based on the player's
	// position. Make sure it does not exceed the bounds of the map.
	camPosX = (player.posX) - (Window::WINDOW_WIDTH / 2);
	if (camPosX < 0) {
		camPosX = 0;
	}
	if (camPosX + Window::WINDOW_WIDTH > width * map.getTileHeight()) {
		camPosX = (map.getWidth() * map.getTileWidth()) - Window::WINDOW_WIDTH;
	}

	// The first tile on the x-axis and how much of it that can
	// be seen.
	firstX = camPosX / map.getTileWidth();
	offsetX = camPosX % map.getTileWidth();

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
				pos.x = (x * map.getTileWidth()) - (offsetX);
				pos.y = y * map.getTileHeight();
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
				pos.x = (x * map.getTileWidth()) - (offsetX);
				pos.y = y * map.getTileHeight();
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
				pos.x = (x * map.getTileWidth()) - (offsetX)-((tempWidth - map.getTileWidth()) / 2);
				pos.y = y * map.getTileHeight() - (tempHeight - map.getTileHeight());
				pos.w = tempWidth;
				pos.h = tempHeight;

				Window::Draw(goalTexture, pos);
			}
		}
	}
}

void Level::drawPlayer()
{
	// Get player position. Adjust to take height/width
	// of the character into account.
	SDL_Rect pos;
	pos.x = player.posX - (player.getWidth() / 2) - camPosX;
	pos.y = player.posY - (player.getHeight() / 2);
	pos.w = player.getWidth();
	pos.h = player.getHeight();

	// Flip the player texture if necessary
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (player.moveState == PLAYER_MOVE_LEFT || player.moveState == PLAYER_STAND_LEFT) {
		flip = SDL_FLIP_HORIZONTAL;
	}

	// Draw the player
	Window::Draw(player.getCurrentTexture(), pos, &player.getCurrentAnimationClip(),
				NULL, NULL, NULL, flip);
}

void Level::drawEnemies()
{
	for (int i = 0; i < numberOfEnemies; i++)
	{
		SDL_RendererFlip flip = SDL_FLIP_NONE;

		Enemy *tempEnemy = enemy2List[i];

		if (tempEnemy->enemyType == ENEMY_FLYING) {
			if (player.posX < tempEnemy->posX) {
				flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (tempEnemy->enemyType == ENEMY_WALKING) {
			if (tempEnemy->moveDirection == ENEMY_MOVE_LEFT) {
				flip = SDL_FLIP_HORIZONTAL;
			}
		}

		SDL_Rect pos;
		pos.x = tempEnemy->posX - (tempEnemy->getWidth() / 2) - camPosX;
		pos.y = tempEnemy->posY - (tempEnemy->getHeight() / 2);
		pos.w = tempEnemy->getWidth();
		pos.h = tempEnemy->getHeight();
		SDL_Texture *tempTex = tempEnemy->getCurrentTexture();
		SDL_Rect tempRect = tempEnemy->getCurrentAnimationClip();

		Window::Draw(tempTex, pos, &tempRect, NULL, NULL, NULL, flip);
	}
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

void Level::loadEnemiesFromMap()
{
	int** tempMap = map.getMap();
	int countEnemies = 0;

	// Count how many enemies that will be spawned.
	for (int row = 0; row < map.getHeight(); row++) {
		for (int col = 0; col < map.getWidth(); col++) {
			if (tempMap[row][col] == ENEMY_FLYING || tempMap[row][col] == ENEMY_WALKING) {
				countEnemies++;
			}
		}
	}

	// Load the enemies.
	this->numberOfEnemies = 0;

	for (int row = 0; row < map.getHeight(); row++) {
		for (int col = 0; col < map.getWidth(); col++) {
			if (tempMap[row][col] == ENEMY_FLYING) {
				EnemyHarpy *newEnemy = new EnemyHarpy();
				newEnemy->loadEnemy(col * map.getTileWidth(), row * map.getTileHeight(), ENEMY_FLYER_HEIGHT,
					ENEMY_FLYER_HEIGHT, ENEMY_FLY_CLIPS, ENEMY_TEXTURE_FLYER);
				enemy2List[numberOfEnemies] = newEnemy;

				this->numberOfEnemies++;
			}

			if (tempMap[row][col] == ENEMY_WALKING) {
				EnemyBear *newEnemy = new EnemyBear();
				newEnemy->loadEnemy(col * map.getTileWidth(), row * map.getTileHeight(), ENEMY_WALKER_HEIGHT, 
					ENEMY_WALKER_WIDTH, ENEMY_WALK_CLIPS, ENEMY_TEXTURE_WALKER);
				enemy2List[numberOfEnemies] = newEnemy;

				this->numberOfEnemies++;
			}
		}
	}
}
