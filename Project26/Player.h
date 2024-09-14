#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
class Player
{
	SDL_Rect rect;
	int score = 0;

public:
	Player(int x, int y, int w, int h) {
		rect.x = x;  // Position of the paddle
		rect.y = y;
		rect.w = w;  // Width of the paddle
		rect.h = h;  // Height of the paddle
	}
	void render(SDL_Renderer* renderer);

	void addScore();

	void reset();

	int getScore();

	SDL_Rect getPlayer();

	pair<int, int> getPos();
	void decrementY(float error = 1.0);
	void incrementY(float error = 1.0);
};

