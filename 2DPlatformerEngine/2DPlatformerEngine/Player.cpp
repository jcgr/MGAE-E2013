#include "Player.h"
#include "Window.h"
// Includes level to get gravity for decelerateY
#include "Level.h"

// PUBLIC
Player::Player()
{
	health = 1;
	posX = 0;
	posY = 0;
	velX = 0;
	velY = 0;
	moveState = PLAYER_STAND_RIGHT;

	canJump = false;
	reachedGoal = false;

	// Initialize the collision points.
	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		collisionPoints[i] = SDL_Point{ 0, 0 };
	}

	currentWalkClip = 0;
	currentJumpClip = 0;
	internalClipCounter = 0;
	internalDeathClipCounter = 0;
}

void Player::loadPlayer()
{
	// Sets initial position to respawn position
	posX = respawnX;
	posY = respawnY;
	isAlive = true;

	// Load the player textures
	playerStand = Window::LoadImage(PLAYER_TEXTURE_STAND);
	playerWalk = Window::LoadImage(PLAYER_TEXTURE_WALK);
	playerJump = Window::LoadImage(PLAYER_TEXTURE_JUMP);
	playerDie = Window::LoadImage(PLAYER_TEXTURE_DIE);

	// Set default texture
	currentTexture = playerStand;

	// Get size of the default texture
	SDL_QueryTexture(currentTexture, NULL, NULL, &playerWidth, &playerHeight);

	animationStandClip.x = 0;
	animationStandClip.y = 0;
	animationStandClip.w = playerWidth;
	animationStandClip.h = playerHeight;

	// Creates the clips for the walking animation
	for (int i = 0; i < PLAYER_WALK_CLIPS; i++)
	{
		animationWalkClips[i].x = i * playerWidth;
		animationWalkClips[i].y = 0;
		animationWalkClips[i].h = playerHeight;
		animationWalkClips[i].w = playerWidth;
	}

	// Creates the clips for the jumping animation
	for (int i = 0; i < PLAYER_JUMP_CLIPS; i++)
	{
		animationJumpClips[i].x = i * playerWidth;
		animationJumpClips[i].y = 0;
		animationJumpClips[i].h = playerHeight;
		animationJumpClips[i].w = playerWidth;
	}

	// Creates the clips for the dying animation
	for (int i = 0; i < PLAYER_DIE_CLIPS; i++)
	{
		animationDieClips[i].x = i * playerWidth;
		animationDieClips[i].y = 0;
		animationDieClips[i].h = playerHeight;
		animationDieClips[i].w = playerWidth;
	}

	// Reduce player size a little to allow for running through holes
	// that are only 1 tile wide.
	playerHeight -= 2;
	playerWidth -= 2;
}

void Player::setSpawn(int x, int y)
{
	respawnX = x;
	respawnY = y;
}

void Player::die()
{
	health = 0;
	isAlive = false;
	currentDeathClip = 0;
}

void Player::respawn()
{
	posX = respawnX;
	posY = respawnY;
	health = 1;
	timeToRespawn = false;
	isAlive = true;
}

void Player::accelerateX()
{
	velX = 7;
	currentWalkClip = 0;
}

void Player::accelerateY()
{
	// After jumping, the player will have to wait with
	// jumping until he has landed.
	if (canJump){
		velY = 200;
		canJump = false;
		isJumping = true;

		// Start with the correct jump clip.
		currentJumpClip = 0;
	}
}

void Player::decelerateX()
{
	if (velX > 0){
		velX--;
		currentWalkClip = 0;
	}

	if (moveState == PLAYER_MOVE_RIGHT) {
		moveState = PLAYER_STAND_RIGHT;
	}

	if (moveState == PLAYER_MOVE_LEFT) {
		moveState = PLAYER_STAND_LEFT;
	}
}

void Player::decelerateY()
{
	velY = -PLAYER_GRAVITY;
}

void Player::updateTexture()
{
	// Resets the internal clip counter.
	internalClipCounter = internalClipCounter % ((PLAYER_WALK_CLIPS - 1) * 2);

	// Calculates the right clip for both animations.
	currentWalkClip = internalClipCounter / 2;
	currentJumpClip = (internalClipCounter - 2) / 2;

	internalClipCounter++;
	if (!isAlive) {
		internalDeathClipCounter++;
		internalDeathClipCounter = internalDeathClipCounter % ((PLAYER_DIE_CLIPS - 1) * 4);
		currentDeathClip = internalDeathClipCounter / 3;
		if (internalDeathClipCounter == ((PLAYER_DIE_CLIPS - 1) * 4) - 1) {
			timeToRespawn = true;
		}
	}

	// Dying animation takes priority over the 
	// jumping animation, which takes priority over the rest.
	if (!isAlive) {
		currentTexture = playerDie;
	} 
	else if (isJumping){
		currentTexture = playerJump;
	}
	else {
		if (moveState == PLAYER_MOVE_RIGHT || moveState == PLAYER_MOVE_LEFT) {
			currentTexture = playerWalk;
		}
		if (moveState == PLAYER_STAND_RIGHT || moveState == PLAYER_STAND_LEFT) {
			currentTexture = playerStand;
		}
	}
}

void Player::move(Map map, Enemy **enemylist, int numberOfEnemies)
{
	// If the player is dead, don't move.
	if (!isAlive) {
		return;
	}

	// If the player if moving left ...
	if (moveState == PLAYER_MOVE_LEFT) {
		for (int i = 0; i < velX; i++)
		{
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX - 1, posY,
				collisionPoints);
			int collisionType = checkCollision(map, enemylist, numberOfEnemies);
			if (collisionType) {
				handleCollision(collisionType);
				break;
			}

			posX--;
		}
	}

	// If the player is moving right ...
	if (moveState == PLAYER_MOVE_RIGHT) {
		for (int i = 0; i < velX; i++)
		{
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX + 1, posY,
				collisionPoints);
			int collisionType = checkCollision(map, enemylist, numberOfEnemies);
			if (collisionType) {
				handleCollision(collisionType);
				break;
			}

			posX++;
		}
	}

	// If the player is moving up ...
	if (velY > 0) {
		// ... check each position step-by-step instead of just moving
		// the player 7 pixels immediately.
		for (int i = 0; i < PLAYER_GRAVITY; i++)
		{
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX, posY - 1,
				collisionPoints);
			int collisionType = checkCollision(map, enemylist, numberOfEnemies);
			if (collisionType) {
				handleCollision(collisionType);
				decelerateY();
				break;
			}

			posY--;
			velY--;
		}
	}
	// If the player is moving down ...
	else {
		for (int i = velY; i < 0; i++)
		{
			// If the player is falling, they cannot jump.
			canJump = false;

			// If the player collides with something below, they have
			// landed and can jump again.
			hc.calculateCollisionPoints(getHeight(), getWidth(),
				posX, posY + 1,
				collisionPoints);
			int collisionType = checkCollision(map, enemylist, numberOfEnemies);
			if (collisionType) {
				handleCollision(collisionType);
				canJump = true;
				isJumping = false;
				break;
			}

			posY++;
		}
	}
}

SDL_Rect Player::getCurrentAnimationClip()
{
	if (!isAlive) {
		return animationDieClips[currentDeathClip];
	}
	else if (isJumping) {
		return animationJumpClips[currentJumpClip];
	}
	else if (moveState == PLAYER_MOVE_LEFT || moveState == PLAYER_MOVE_RIGHT) {
		return animationWalkClips[currentWalkClip];
	}
	else {
		return animationStandClip;
	}
}

SDL_Texture* Player::getCurrentTexture()
{
	return currentTexture;
}

int Player::getHeight()
{
	return playerHeight;
}

int Player::getWidth()
{
	return playerWidth;
}

int Player::checkCollision(Map map, Enemy **enemylist, int numberOfEnemies)
{
	int tileType = 0;

	// Check if the player hits an enemy
	for (int point = 0; point < COLLISION_POINT_AMOUNT; point++)
	{
		SDL_Point tempPoint = collisionPoints[point];

		for (int enemy = 0; enemy < numberOfEnemies; enemy++)
		{
			Enemy *tempEnemy = enemylist[enemy];

			if (tempPoint.x > tempEnemy->posX && tempPoint.x < tempEnemy->posX + tempEnemy->getWidth()
				&& tempPoint.y > tempEnemy->posY && tempPoint.y < tempEnemy->posY + tempEnemy->getHeight()) {
				return COLLISION_ENEMY;
			}
		}
	}

	// Iterate over all collision points to check for collision with the map
	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		SDL_Point tempPoint = collisionPoints[i];
		int tempCollisionType = map.getTile(tempPoint.x, tempPoint.y);

		// If checking for the player, just return the highest collision type
		if (tempCollisionType > tileType) {
			tileType = tempCollisionType;
		}
	}

	return tileType;
}

void Player::handleCollision(int collisionType)
{
	switch (collisionType)
	{
	case COLLISION_SOLID_BLOCK:
		// Do nothing. Is handled depending on where it happens
		break;

	case COLLISION_GOAL:
		reachedGoal = true;
		break;

	case COLLISION_SPIKE:
		die();
		break;

	case COLLISION_ENEMY:
		die();
		break;
	}
}

// PRIVATE