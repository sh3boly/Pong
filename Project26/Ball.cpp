#include "Ball.h"

void Ball::reverseDir()
{
	dir *= -1;
}

void Ball::setYDir(int sign) {
	yDir = sign;
}

void Ball::reverseYDir()
{
	yDir *= -1;
}

void Ball::move()
{
	rect.x = rect.x - (5 * dir); 
	rect.y = rect.y + (5 * yFactor * yDir);
}

SDL_Rect Ball::getBall()
{
	return rect;
}

void Ball::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void Ball::reset()
{
	rect.x = 1280 / 2;
	rect.y = 720 / 2;
}
