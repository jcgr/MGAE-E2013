#include "Player.h"
#include "Window.h"

// PUBLIC
Player::Player()
{
	Player(300, 300);
}

Player::Player(int x, int y)
{
	health = 1;
	posX = x;
	posY = y;
	velX = 0;
	velY = 0;
	moveState = STAND_RIGHT;

	canJump = false;
	reachedGoal = false;

	// Initialize the collision points.
	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		SDL_Point point = { 0, 0 };
		collisionPoints[i] = point;
	}

	currentWalkClip = 0;
	currentJumpClip = 0;
	internalClipCounter = 0;
}

void Player::loadPlayer()
{
	// Load the player textures
	playerStandRight = Window::LoadImage("media/SamusStandRight64px.png");
	playerStandLeft = Window::LoadImage("media/SamusStandLeft64px.png");
	playerWalkRight = Window::LoadImage("media/SamusWalkRight64px.png");
	playerWalkLeft = Window::LoadImage("media/SamusWalkLeft64px.png");
	playerJumpRight = Window::LoadImage("media/SamusJumpRight64px.png");
	playerJumpLeft = Window::LoadImage("media/SamusJumpLeft64px.png");

	// Set default texture
	currentTexture = playerStandRight;

	// Get size of the default texture
	SDL_QueryTexture(playerStandRight, NULL, NULL, &playerWidth, &playerHeight);

	calculateCollisionPoints(posX, posY, collisionPoints);

	animationStandClip.x = 0;
	animationStandClip.y = 0;
	animationStandClip.w = playerWidth;
	animationStandClip.h = playerHeight;

	// Creates the clips for the walking animation
	for (int i = 0; i < 10; i++)
	{
		animationWalkClips[i].x = i * playerWidth;
		animationWalkClips[i].y = 0;
		animationWalkClips[i].h = playerHeight;
		animationWalkClips[i].w = playerWidth;
	}

	// Creates the clips for the jumping animation
	for (int i = 0; i < 8; i++)
	{
		animationJumpClips[i].x = i * playerWidth;
		animationJumpClips[i].y = 0;
		animationJumpClips[i].h = playerHeight;
		animationJumpClips[i].w = playerWidth;
	}
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

	if (moveState == MOVE_RIGHT) {
		moveState = STAND_RIGHT;
	}

	if (moveState == MOVE_LEFT) {
		moveState = STAND_LEFT;
	}
}

void Player::decelerateY()
{
	velY = -7;
}

int Player::getPosX()
{
	return posX;
}

int Player::getPosY()
{
	return posY;
}

SDL_Rect Player::getCurrentAnimationClip()
{
	if (isJumping){
		return animationJumpClips[currentJumpClip];
	}
	else if (moveState == MOVE_LEFT || moveState == MOVE_RIGHT) {
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

void Player::move(Map map)
{
	// Resets the internal clip counter.
	if (internalClipCounter > 18){
		internalClipCounter = 0;
	}

	// Calculates the right clip for both animations.
	currentWalkClip = internalClipCounter / 2;
	currentJumpClip = (internalClipCounter - 2) / 2;

	// Moves the player if necessary
	moveHorizontal(map);
	moveVertical(map);

	internalClipCounter++;
	updateTexture();

	// Calculates new collision points for the player after moving.
	calculateCollisionPoints(posX, posY, collisionPoints);
}

// !PUBLIC

// PRIVATE

void Player::moveHorizontal(Map map)
{
	if (moveState == MOVE_LEFT){
		// Smooth collision detection.
		for (int i = 0; i < velX; i++)
		{
			if (isColliding(map, posX - 1, posY)) {
				break;
			}

			posX--;
		}
	}
	if (moveState == MOVE_RIGHT){
		// Smooth collision detection.
		for (int i = 0; i < velX; i++)
		{
			if (isColliding(map, posX + 1, posY)) {
				break;
			}

			posX++;
		}
	}
}

void Player::moveVertical(Map map)
{
	// If the player is moving up ...
	if (velY > 0) {
		// ... check each position step-by-step instead of just moving
		// the player 7 pixels immediately.
		for (int i = 0; i < 7; i++)
		{
			if (isColliding(map, posX, posY - 1)) {
				decelerateY();
				break;
			}

			posY--;
			velY--;
		}
	} else {
		for (int i = velY; i < 0; i++)
		{
			// If the player collides with something below, he has
			// landed and can jump again.
			if (isColliding(map, posX, posY + 1)) {
				canJump = true;
				isJumping = false;
				break;
			}

			posY++;
		}
	}
}

void Player::updateTexture()
{
	// Jumping animation takes priority.
	if (isJumping){
		if (moveState == MOVE_RIGHT || moveState == STAND_RIGHT){
			currentTexture = playerJumpRight;
		}
		if (moveState == MOVE_LEFT || moveState == STAND_LEFT){
			currentTexture = playerJumpLeft;
		}
	} else {
		switch (moveState)
		{
		case MOVE_RIGHT:
			currentTexture = playerWalkRight;
			break;

		case MOVE_LEFT:
			currentTexture = playerWalkLeft;
			break;

		case STAND_RIGHT:
			currentTexture = playerStandRight;
			break;

		case STAND_LEFT:
			currentTexture = playerStandLeft;
			break;

		default:
			break;
		}
	}
}

void Player::calculateCollisionPoints(int newPosX, int newPosY, SDL_Point* cPoints)
{
	// Calculate 9 collision points. Each corner, midways between
	// each corner and the middle of the texture.
	for (int i = 0; i < 9; i++)
	{
		SDL_Point point;
		point.x = newPosX + ((i / 3) * (playerWidth / 2));
		point.y = newPosY + ((i % 3) * (playerHeight / 2));

		cPoints[i] = point;
	}
}

bool Player::isColliding(Map map, int newPosX, int newPosY)
{
	bool isColliding = false;

	SDL_Point collisionDetection[COLLISION_POINT_AMOUNT];
	calculateCollisionPoints(newPosX, newPosY, collisionDetection);

	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		if (!isColliding){
			SDL_Point tempPoint = collisionDetection[i];
			isColliding = map.isPixelSolid(tempPoint.x, tempPoint.y);
		}
	}

	return isColliding;
}

int Player::mapCollision(Map map, int newPosX, int newPosY)
{
	int tileType;

	SDL_Point collisionDetection[COLLISION_POINT_AMOUNT];
	calculateCollisionPoints(newPosX, newPosY, collisionDetection);

	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		if (!tileType){
			SDL_Point tempPoint = collisionDetection[i];
			tileType = map.getTile(tempPoint.x, tempPoint.y);
		}
	}

	return tileType;
}

void Player::handleCollision(int collisionType)
{
	switch (collisionType)
	{
	case TILE_SOLID_BLOCK:
		// Do nothing. Is handled depending on where it happens
		break;
		
	case TILE_GOAL:
		reachedGoal = true;
		break;
	}

}
// !PRIVATE