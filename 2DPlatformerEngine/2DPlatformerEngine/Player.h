#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "Map.h"

/**
* The player
*
* TODO: Dynamic animationclips.
*/
class Player
{
	public:
		Player();
		~Player(){};

		#define COLLISION_POINT_AMOUNT 9
		#define STAND_RIGHT 1
		#define STAND_LEFT -1
		#define MOVE_RIGHT 2
		#define MOVE_LEFT -2

		int health;
		/**
		* One of the coordinates for the player's pixel position.
		*/
		int posX, posY;
		/**
		* The state of movement the player is in.
		*/
		int moveState;

		/**
		* A value that tells if the player can jump or not.
		*/
		bool canJump;
		/**
		* A value that tells if the player has reached the goal or not.
		*/
		bool reachedGoal;
		/**
		* A value that tells if the player is jumping or not.
		*/
		bool isJumping;

		/**
		* The points to check for collision with regards to the player.
		*/
		SDL_Point currentCollisionPoints[COLLISION_POINT_AMOUNT], tempCollisionPoints[COLLISION_POINT_AMOUNT];

		/**
		* Uses the given texture to determine the height
		* and width of the player.
		* @param playerTexture The default texture of the player.
		*/
		void loadPlayer();
		/**
		* Sets the spawn point of the player to the given position.
		* Does NOT check if there is a wall.
		* @param x The x-coordinate
		* @param y The y-coordinate
		*/
		void setSpawn(int x, int y);

		/**
		* Increases the player's x velocity
		*/
		void accelerateX();
		/**
		* Increases the player's y velocity
		*/
		void accelerateY();
		/**
		* Decreases the player's x velocity
		*/
		void decelerateX();
		/**
		* Decreases the player's y velocity
		*/
		void decelerateY();

		/**
		* Updates the player's texture to correspond to what the
		* player is doing.
		*/
		void updateTexture();

		/**
		* Gets the height of the player
		* @return The height of the player
		*/
		int getHeight();
		/**
		* Gets the width of the player
		* @return The width of the player
		*/
		int getWidth();

		/**
		* Gets the x velocity of the player
		* @return The x velocity of the player
		*/
		int getVelX();
		/**
		* Gets the y velocity of the player
		* @return The y velocity of the player
		*/
		int getVelY();
		/**
		* Sets the value of the player's y velocity
		* @param y The new value
		*/
		void setVelY(int y);
		/**
		* Sets the value of the player's x velocity
		* @param x The new value
		*/
		void setVelX(int x);

		/**
		* Gets the rectangle that take corresponds to the right
		* part of the current animation.
		* @return The rectangle with the right values
		*/
		SDL_Rect getCurrentAnimationClip();

		/**
		* Gets the current texture.
		* @return The current player texture.
		*/
		SDL_Texture* getCurrentTexture();

	private:

		/**
		* The player's respawn point.
		*/
		int respawnX, respawnY;
		/**
		* The player's velocity along an axis.
		*/
		int velX, velY;
		/**
		* The size of the player.
		*/
		int playerHeight, playerWidth;
		/**
		* Used to determine the current animation.
		*/
		int currentWalkClip, currentJumpClip;
		/**
		* The internal clip counter for deciding what animation to use.
		* Used to slow down the switching of clips for drawing.
		*/
		int internalClipCounter;

		/**
		* The clip for the standing animation
		*/
		SDL_Rect animationStandClip;
		/**
		* The clips for the walking animation
		*/
		SDL_Rect animationWalkClips[10];
		/**
		* The clips for the jumping animation
		*/
		SDL_Rect animationJumpClips[8];

		/**
		* The texture used for drawing the player.
		*/
		SDL_Texture *currentTexture;
		/**
		* The texture that shows the player standing.
		*/
		SDL_Texture *playerStandRight, *playerStandLeft;
		/**
		* The texture that shows the player walking/running.
		*/
		SDL_Texture *playerWalkRight, *playerWalkLeft;
		/**
		* The texture that shows the player jumping.
		*/
		SDL_Texture *playerJumpRight, *playerJumpLeft;
};

#endif // !PLAYER_H