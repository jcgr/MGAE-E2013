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

void HelperClass::drawLevelVictoryScreen()
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

void HelperClass::drawGameWonScreen()
{
	SDL_Color white = { 255, 255, 255 };
	SDL_Texture *msgGrats, *msgContinue;
	SDL_Rect msgGratsBox, msgContinueBox;

	msgGrats = Window::RenderText("No more levels to load! You have won!", "FreeSans.ttf", white, 50);
	msgContinue = Window::RenderText("Press [escape] to stop the game.", "FreeSans.ttf", white, 30);

	SDL_QueryTexture(msgGrats, NULL, NULL, &msgGratsBox.w, &msgGratsBox.h);
	SDL_QueryTexture(msgContinue, NULL, NULL, &msgContinueBox.w, &msgContinueBox.h);

	msgGratsBox.x = (Window::Box().w / 2) - (msgGratsBox.w / 2);
	msgGratsBox.y = (Window::Box().h / 2) - (msgGratsBox.h / 2) - 25;

	msgContinueBox.x = (Window::Box().w / 2) - (msgContinueBox.w / 2);
	msgContinueBox.y = (Window::Box().h / 2) - (msgContinueBox.h / 2) + 25;

	Window::Draw(msgGrats, msgGratsBox);
	Window::Draw(msgContinue, msgContinueBox);
}