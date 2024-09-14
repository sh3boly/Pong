#pragma once
#include <SDL.h>
class Ball
{
	SDL_Rect rect;
	int dir = 1;
	int yDir = 1;
	int yFactor = 1;
public:
	Ball() {
		rect.x = 1280 / 2;
		rect.y = 720 / 2;
		rect.w = 10;
		rect.h = 10;
	}
	void reverseDir();
	void setYDir(int sign);
	void reverseYDir();
	void move();
	SDL_Rect getBall();
	void render(SDL_Renderer* renderer);
	void reset();
};

