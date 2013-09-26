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

// PRIVATE