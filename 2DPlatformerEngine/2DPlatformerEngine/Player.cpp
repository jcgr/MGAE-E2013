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
	moveState = STAND_RIGHT;

	canJump = false;
	reachedGoal = false;

	// Initialize the collision points.
	for (int i = 0; i < COLLISION_POINT_AMOUNT; i++)
	{
		currentCollisionPoints[i] = SDL_Point{ 0, 0 };
		tempCollisionPoints[i] = SDL_Point{ 0, 0 };
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
	playerStandRight = Window::LoadImage("media/SamusStandRight64px.png");
	playerStandLeft = Window::LoadImage("media/SamusStandLeft64px.png");
	playerWalkRight = Window::LoadImage("media/SamusWalkRight64px.png");
	playerWalkLeft = Window::LoadImage("media/SamusWalkLeft64px.png");
	playerJumpRight = Window::LoadImage("media/SamusJumpRight64px.png");
	playerJumpLeft = Window::LoadImage("media/SamusJumpLeft64px.png");
	playerDie = Window::LoadImage("media/SamusDie64px.png");

	// Set default texture
	currentTexture = playerStandRight;

	// Get size of the default texture
	SDL_QueryTexture(currentTexture, NULL, NULL, &playerWidth, &playerHeight);

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

	// Creates the clips for the dying animation
	for (int i = 0; i < 15; i++)
	{
		animationDieClips[i].x = i * playerWidth;
		animationDieClips[i].y = 0;
		animationDieClips[i].h = playerHeight;
		animationDieClips[i].w = playerWidth;
	}

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

	if (moveState == MOVE_RIGHT) {
		moveState = STAND_RIGHT;
	}

	if (moveState == MOVE_LEFT) {
		moveState = STAND_LEFT;
	}
}

void Player::decelerateY()
{
	velY = -GRAVITY;
}

void Player::updateTexture()
{
	// Resets the internal clip counter.
	internalClipCounter = internalClipCounter % 18;

	// Calculates the right clip for both animations.
	currentWalkClip = internalClipCounter / 2;
	currentJumpClip = (internalClipCounter - 2) / 2;

	internalClipCounter++;
	if (!isAlive) {
		internalDeathClipCounter++;
		internalDeathClipCounter = internalDeathClipCounter % 56;
		currentDeathClip = internalDeathClipCounter / 3;
		if (internalDeathClipCounter == 55) {
			timeToRespawn = true;
		}
	}

	// Dying animation takes priority over the 
	// jumping animation, which takes priority over the rest.
	if (!isAlive) {
		currentTexture = playerDie;
	} 
	else if (isJumping){
		if (moveState == MOVE_RIGHT || moveState == STAND_RIGHT){
			currentTexture = playerJumpRight;
		}
		if (moveState == MOVE_LEFT || moveState == STAND_LEFT){
			currentTexture = playerJumpLeft;
		}
	}
	else {
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

SDL_Rect Player::getCurrentAnimationClip()
{
	if (!isAlive) {
		return animationDieClips[currentDeathClip];
	}
	else if (isJumping) {
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

int Player::getHeight()
{
	return playerHeight;
}

int Player::getWidth()
{
	return playerWidth;
}

// PRIVATE