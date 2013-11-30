#include "HelperClass.h"

void HelperClass::calculateCollisionPoints(int unitHeight, int unitWidth, int newPosX, int newPosY, SDL_Point* collisionPoints)
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