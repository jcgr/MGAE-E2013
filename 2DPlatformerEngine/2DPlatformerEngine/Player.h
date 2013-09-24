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
		Player(int x, int y);
		~Player(){};

		#define COLLISION_POINT_AMOUNT 9
		#define STAND_RIGHT 1
		#define STAND_LEFT -1
		#define MOVE_RIGHT 2
		#define MOVE_LEFT -2

		/**
		* The state of movement the player is in.
		*/
		int moveState;
		/**
		* The size of the player.
		*/
		int playerHeight, playerWidth;

		/**
		* A value that tells if the player is jumping or not.
		*/
		bool isJumping;

		/**
		* The points to check for collision with regards to the player.
		*/
		SDL_Point collisionPoints[COLLISION_POINT_AMOUNT];

		/**
		* Uses the given texture to determine the height
		* and width of the player.
		* @param playerTexture The default texture of the player.
		*/
		void loadPlayer();

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
		* Tells the player to move
		* @param map The map to use for collision detection.
		*/
		void move(Map map);

		/**
		* Gets the x-coordinate of the player's position.
		* @return The player's pixel position on the x-axis.
		*/
		int getPosX();
		/**
		* Gets the y-coordinate of the player's position.
		* @return The player's pixel position on the y-axis.
		*/
		int getPosY();
		/**
		* Gets the player's velocity on the x-axis.
		* @return The player's velocity on the x-axis.
		*/
		int getVelX();
		/**
		* Gets the player's velocity on the y-axis.
		* @return The player's velocity on the y-axis.
		*/
		int getVelY();

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

		int health;
		/**
		* One of the coordinates for the player's pixel position.
		*/
		int posX, posY;
		/**
		* The player's velocity along an axis.
		*/
		int velX, velY;
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
		* A value that tells if the player can jump or not.
		*/
		bool canJump;
		bool reachedGoal;

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

		/**
		* Moves the player horizontally.
		* @param map The map used for collision detection.
		*/
		void moveHorizontal(Map map);
		/**
		* Moves the player vertically.
		* @param map The map used for collision detection.
		*/
		void moveVertical(Map map);

		/**
		* Updates the player's texture to correspond to what the
		* player is doing.
		*/
		void updateTexture();

		/**
		* Calculates the collision points for the given position.
		* @param newPosX The x coordiate of the new position.
		* @param newPosY The y coordiate of the new position.
		* @param collisionPoints The array to put the results into.
		*/
		void calculateCollisionPoints(int newPosX, int newPosY, SDL_Point* collisionPoints);

		/**
		* Calculates if the player is colliding with something in the map.
		* @param map The map to check for collision with. 
		* @param newPosX The x-coordinate of the position to check for.
		* @param newPosY The y-coordinate of the position to check for.
		* @return True is the player will collide with something solid; false if not.
		*/
		bool isColliding(Map map, int newPosX, int newPosY);
		int mapCollision(Map map, int newPosX, int newPosY);

		void handleCollision(int collisionType);
};

#endif // !PLAYER_H