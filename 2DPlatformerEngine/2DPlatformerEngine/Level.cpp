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
	calculateCollisionPoints(player.getHeight(), player.getWidth(), player.posX, player.posY, player.currentCollisionPoints);

	loadEnemiesFromMap();

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

	movePlayer();
	player.updateTexture();
	calculateCollisionPoints(player.getHeight(), player.getWidth(), player.posX, player.posY, player.currentCollisionPoints);
}

void Level::updateMap()
{
	// Does nothing
}

void Level::updateEnemies()
{
	// If the player is dead, don't bother moving
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
		case ENEMY_HARPY:
			tempEnemy = moveHarpy(tempEnemy);
			break;

		case ENEMY_GRIZZLY:
			tempEnemy = moveGrizzly(tempEnemy);
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
	if (player.moveState == MOVE_LEFT) {
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
	if (player.moveState == MOVE_RIGHT) {
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
		for (int i = 0; i < GRAVITY; i++)
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

Enemy Level::moveHarpy(Enemy harpy)
{
	for (int movement = 0; movement < harpy.velY; movement++)
	{
		if (harpy.moveDirection == MOVE_UP)
		{
			calculateCollisionPoints(harpy.getHeight(), harpy.getWidth(),
				harpy.posX, harpy.posY - 1,
				harpy.tempCollisionPoints);
			int collisionType = checkCollision(harpy.tempCollisionPoints, false);
			if (collisionType > 0) {
				harpy.moveDirection = MOVE_DOWN;
				break;
			}

			harpy.posY--;
		}
		else if (harpy.moveDirection == MOVE_DOWN)
		{
			calculateCollisionPoints(harpy.getHeight(), harpy.getWidth(),
				harpy.posX, harpy.posY + 1,
				harpy.tempCollisionPoints);
			int collisionType = checkCollision(harpy.tempCollisionPoints, false);
			if (collisionType > 0) {
				harpy.moveDirection = MOVE_UP;
				break;
			}

			harpy.posY++;
		}
	}

	//if (harpy.moveDirection == MOVE_UP)
	//{
	//	for (int movement = 0; movement < harpy.velY; movement++)
	//	{
	//		calculateCollisionPoints(harpy.getHeight(), harpy.getWidth(),
	//			harpy.posX, harpy.posY - 1,
	//			harpy.tempCollisionPoints);
	//		int collisionType = checkCollision(harpy.tempCollisionPoints, false);
	//		if (collisionType > 0) {
	//			harpy.moveDirection = MOVE_DOWN;
	//			break;
	//		}

	//		harpy.posY--;
	//	}
	//} 
	//else if (harpy.moveDirection == MOVE_DOWN)
	//{
	//	for (int movement = 0; movement < harpy.velY; movement++)
	//	{
	//		calculateCollisionPoints(harpy.getHeight(), harpy.getWidth(),
	//			harpy.posX, harpy.posY + 1,
	//			harpy.tempCollisionPoints);
	//		int collisionType = checkCollision(harpy.tempCollisionPoints, false);
	//		if (collisionType > 0) {
	//			harpy.moveDirection = MOVE_UP;
	//			break;
	//		}

	//		harpy.posY++;
	//	}
	//}

	return harpy;
}

Enemy Level::moveGrizzly(Enemy grizzly)
{
	// Move the grizzly horizontally based on its moveDirection + velocity.
	for (int movement = 0; movement < grizzly.velX; movement++)
	{
		if (grizzly.moveDirection == MOVE_RIGHT)
		{
			// Check for collision
			calculateCollisionPoints(grizzly.getHeight(), grizzly.getWidth(),
				grizzly.posX + 1, grizzly.posY,
				grizzly.tempCollisionPoints);
			int collisionType = checkCollision(grizzly.tempCollisionPoints, false);
			if (collisionType > 0) {
				grizzly.moveDirection = MOVE_LEFT;
				break;
			}

			grizzly.posX++;
		}
		else if (grizzly.moveDirection == MOVE_LEFT)
		{
			// Check for collision
			calculateCollisionPoints(grizzly.getHeight(), grizzly.getWidth(),
				grizzly.posX - 1, grizzly.posY,
				grizzly.tempCollisionPoints);
			int collisionType = checkCollision(grizzly.tempCollisionPoints, false);
			if (collisionType > 0) {
				grizzly.moveDirection = MOVE_RIGHT;
				break;
			}

			grizzly.posX--;
		}
	}
	
	// Move the grizzly vertically
	for (int movement = 0; movement < grizzly.velY; movement++)
	{
		// Check for collision
		calculateCollisionPoints(grizzly.getHeight(), grizzly.getWidth(),
			grizzly.posX, grizzly.posY + 1,
			grizzly.tempCollisionPoints);
		int collisionType = checkCollision(grizzly.tempCollisionPoints, false);
		if (collisionType > 0) {
			grizzly.isFalling = false;
			break;
		}

		grizzly.isFalling = true;
		grizzly.posY++;
	}

	return grizzly;
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
	// Get player position. Adjust to take height/width
	// of the character into account.
	SDL_Rect pos;
	pos.x = player.posX - (player.getWidth() / 2) - camPosX;
	pos.y = player.posY - (player.getHeight() / 2);
	pos.w = player.getWidth();
	pos.h = player.getHeight();

	// Flip the player texture if necessary
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (player.moveState == MOVE_LEFT || player.moveState == STAND_LEFT) {
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

		if (tempEnemy.enemyType == ENEMY_HARPY) {
			if (player.posX < tempEnemy.posX) {
				flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (tempEnemy.enemyType == ENEMY_GRIZZLY) {
			if (tempEnemy.moveDirection == MOVE_LEFT) {
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
			if (tempMap[row][col] == ENEMY_HARPY || tempMap[row][col] == ENEMY_GRIZZLY) {
				countEnemies++;
			}
		}
	}

	// Load the enemies correctly.
	numberOfEnemies = 0;
	enemyList = new Enemy[countEnemies];
	for (int row = 0; row < map.getHeight(); row++) {
		for (int col = 0; col < map.getWidth(); col++) {
			if (tempMap[row][col] == ENEMY_HARPY) {
				Enemy newEnemy = Enemy();
				newEnemy.loadEnemy(col * TILE_WIDTH, row * TILE_HEIGHT, ENEMY_HARPY);
				enemyList[numberOfEnemies] = newEnemy;
				numberOfEnemies++;
			}
			if (tempMap[row][col] == ENEMY_GRIZZLY) {
				Enemy newEnemy = Enemy();
				newEnemy.loadEnemy(col * TILE_WIDTH, row * TILE_HEIGHT, ENEMY_GRIZZLY);
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