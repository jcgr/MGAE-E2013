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
	map = Map();
	map.loadMap(level);
	SDL_Rect initialSpawn = map.getPlayerSpawn();

	// Loads the player.
	player = Player();
	player.setSpawn(initialSpawn.x, initialSpawn.y);
	player.loadPlayer();
	calculateCollisionPoints(player.getHeight(), player.getWidth(), player.posX, player.posY, player.currentCollisionPoints);

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

		updateMap();
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
	movePlayer();
	calculateCollisionPoints(player.getHeight(), player.getWidth(), player.posX, player.posY, player.currentCollisionPoints);
}

void Level::updateMap()
{
	// Does nothing
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
		Enemy tempEnemy = enemyList[enemy];
		tempEnemy.updateTexture();
		tempEnemy.updateSpeed();

		switch (tempEnemy.enemyType)
		{
		case ENEMY_FLYING:
			tempEnemy = moveFlying(tempEnemy);
			break;

		case ENEMY_WALKING:
			tempEnemy = moveWalking(tempEnemy);
			break;

		default:
			break;
		}

		enemyList[enemy] = tempEnemy;
	}
}

void Level::movePlayer()
{
	// If the player is dead, don't move.
	if (!player.isAlive) {
		return;
	}

	// If the player if moving left ...
	if (player.moveState == PLAYER_MOVE_LEFT) {
		for (int i = 0; i < player.velX; i++)
		{
			calculateCollisionPoints(player.getHeight(), player.getWidth(), 
									player.posX - 1, player.posY, 
									player.tempCollisionPoints);
			int collisionType = checkCollision(player.tempCollisionPoints, true);
			if (collisionType) {
				handlePlayerCollision(collisionType);
				break;
			}

			player.posX--;
		}
	}

	// If the player is moving right ...
	if (player.moveState == PLAYER_MOVE_RIGHT) {
		for (int i = 0; i < player.velX; i++)
		{
			calculateCollisionPoints(player.getHeight(), player.getWidth(),
									player.posX + 1, player.posY,
									player.tempCollisionPoints);
			int collisionType = checkCollision(player.tempCollisionPoints, true);
			if (collisionType) {
				handlePlayerCollision(collisionType);
				break;
			}

			player.posX++;
		}
	}

	// If the player is moving up ...
	if (player.velY > 0) {
		// ... check each position step-by-step instead of just moving
		// the player 7 pixels immediately.
		for (int i = 0; i < PLAYER_GRAVITY; i++)
		{
			calculateCollisionPoints(player.getHeight(), player.getWidth(),
				player.posX, player.posY - 1,
				player.tempCollisionPoints);
			int collisionType = checkCollision(player.tempCollisionPoints, true);
			if (collisionType) {
				handlePlayerCollision(collisionType);
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
			calculateCollisionPoints(player.getHeight(), player.getWidth(),
									player.posX, player.posY + 1,
									player.tempCollisionPoints);
			int collisionType = checkCollision(player.tempCollisionPoints, true);
			if (collisionType) {
				handlePlayerCollision(collisionType);
				player.canJump = true;
				player.isJumping = false;
				break;
			}

			player.posY++;
		}
	}
}

Enemy Level::moveFlying(Enemy flyer)
{
	for (int movement = 0; movement < flyer.velY; movement++)
	{
		if (flyer.moveDirection == ENEMY_MOVE_UP)
		{
			calculateCollisionPoints(flyer.getHeight(), flyer.getWidth(),
				flyer.posX, flyer.posY - 1,
				flyer.tempCollisionPoints);
			int collisionType = checkCollision(flyer.tempCollisionPoints, false);
			if (collisionType > 0) {
				flyer.moveDirection = ENEMY_MOVE_DOWN;
				break;
			}

			flyer.posY--;
		}
		else if (flyer.moveDirection == ENEMY_MOVE_DOWN)
		{
			calculateCollisionPoints(flyer.getHeight(), flyer.getWidth(),
				flyer.posX, flyer.posY + 1,
				flyer.tempCollisionPoints);
			int collisionType = checkCollision(flyer.tempCollisionPoints, false);
			if (collisionType > 0) {
				flyer.moveDirection = ENEMY_MOVE_UP;
				break;
			}

			flyer.posY++;
		}
	}

	return flyer;
}

Enemy Level::moveWalking(Enemy walker)
{
	// Move the walker horizontally based on its moveDirection + velocity.
	for (int movement = 0; movement < walker.velX; movement++)
	{
		if (walker.moveDirection == ENEMY_MOVE_RIGHT)
		{
			// Check for collision
			calculateCollisionPoints(walker.getHeight(), walker.getWidth(),
				walker.posX + 1, walker.posY,
				walker.tempCollisionPoints);
			int collisionType = checkCollision(walker.tempCollisionPoints, false);
			if (collisionType > 0) {
				walker.moveDirection = ENEMY_MOVE_LEFT;
				break;
			}

			walker.posX++;
		}
		else if (walker.moveDirection == ENEMY_MOVE_LEFT)
		{
			// Check for collision
			calculateCollisionPoints(walker.getHeight(), walker.getWidth(),
				walker.posX - 1, walker.posY,
				walker.tempCollisionPoints);
			int collisionType = checkCollision(walker.tempCollisionPoints, false);
			if (collisionType > 0) {
				walker.moveDirection = ENEMY_MOVE_RIGHT;
				break;
			}

			walker.posX--;
		}
	}
	
	// Make the walker fall
	for (int movement = 0; movement < walker.velY; movement++)
	{
		// Check for collision
		calculateCollisionPoints(walker.getHeight(), walker.getWidth(),
			walker.posX, walker.posY + 1,
			walker.tempCollisionPoints);
		int collisionType = checkCollision(walker.tempCollisionPoints, false);
		if (collisionType > 0) {
			walker.isFalling = false;
			break;
		}

		walker.isFalling = true;
		walker.posY++;
	}

	return walker;
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

		Enemy tempEnemy = enemyList[i];

		if (tempEnemy.enemyType == ENEMY_FLYING) {
			if (player.posX < tempEnemy.posX) {
				flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (tempEnemy.enemyType == ENEMY_WALKING) {
			if (tempEnemy.moveDirection == ENEMY_MOVE_LEFT) {
				flip = SDL_FLIP_HORIZONTAL;
			}
		}

		SDL_Rect pos;
		pos.x = tempEnemy.posX - (tempEnemy.getWidth() / 2) - camPosX;
		pos.y = tempEnemy.posY - (tempEnemy.getHeight() / 2);
		pos.w = tempEnemy.getWidth();
		pos.h = tempEnemy.getHeight();
		Window::Draw(tempEnemy.getCurrentTexture(), pos, &tempEnemy.getCurrentAnimationClip(),
			NULL, NULL, NULL, flip);
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

	// Load the enemies correctly.
	numberOfEnemies = 0;
	enemyList = new Enemy[countEnemies];
	for (int row = 0; row < map.getHeight(); row++) {
		for (int col = 0; col < map.getWidth(); col++) {
			if (tempMap[row][col] == ENEMY_FLYING) {
				Enemy newEnemy = Enemy();
				newEnemy.loadEnemy(col * map.getTileWidth(), row * map.getTileHeight(), ENEMY_FLYING);
				enemyList[numberOfEnemies] = newEnemy;
				numberOfEnemies++;
			}
			if (tempMap[row][col] == ENEMY_WALKING) {
				Enemy newEnemy = Enemy();
				newEnemy.loadEnemy(col * map.getTileWidth(), row * map.getTileHeight(), ENEMY_WALKING);
				enemyList[numberOfEnemies] = newEnemy;
				numberOfEnemies++;
			}
		}
	}
}

void Level::calculateCollisionPoints(int unitHeight, int unitWidth, int newPosX, int newPosY, SDL_Point* collisionPoints)
{
	// Calculate 9 collision points. Each corner, midways between
	// each corner and the middle of the texture.
	for (int i = 0; i < 9; i++)
	{
		SDL_Point point;
		point.x = newPosX + ((i / 3) * (unitWidth / 2));
		point.y = newPosY + ((i % 3) * (unitHeight / 2));

		collisionPoints[i] = point;
	}
}

void Level::handlePlayerCollision(int collisionType)
{
	switch (collisionType)
	{
	case COLLISION_SOLID_BLOCK:
		// Do nothing. Is handled depending on where it happens
		break;

	case COLLISION_GOAL:
		player.reachedGoal = true;
		break;

	case COLLISION_SPIKE:
		player.die();
		break;

	case COLLISION_ENEMY:
		player.die();
		break;
	}
}

int Level::checkCollision(SDL_Point *collisionPoints, bool player)
{
	int tileType = 0;

	// Check if the player hits an enemy
	if (player)
	{
		for (int point = 0; point < COLLISION_POINT_AMOUNT; point++)
		{
			SDL_Point tempPoint = collisionPoints[point];

			for (int enemy = 0; enemy < numberOfEnemies; enemy++)
			{
				Enemy tempEnemy = enemyList[enemy];

				if (tempPoint.x > tempEnemy.posX && tempPoint.x < tempEnemy.posX + tempEnemy.getWidth()
					&& tempPoint.y > tempEnemy.posY && tempPoint.y < tempEnemy.posY + tempEnemy.getHeight()) {
						return COLLISION_ENEMY;
				}
			}
		}
	}

	// Iterate over all collision points to check for collision with the map
	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		SDL_Point tempPoint = collisionPoints[i];
		int tempCollisionType = map.getTile(tempPoint.x, tempPoint.y);

		// If checking for the player, just return the highest collision type
		if (player) {
			if (tempCollisionType > tileType) {
				tileType = tempCollisionType;
			}
		}
		// If checking for enemies, change only if it hits bricks or spikes
		else {
			if (tempCollisionType == COLLISION_GOAL) { 
				// Do nothing 
			}
			else if (tempCollisionType != COLLISION_EMPTY) {
				tileType = tempCollisionType;
			}
		}
	}

	return tileType;
}