#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"
#include "Ball.h"
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;                   // Desired frame rate
const int frameDelay = 1000 / FPS;    // Frame duration in milliseconds

Uint32 frameStart;                    // Start time of the frame
int frameTime;                        // Time taken by the current frame

TTF_Font* font = NULL;
SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
Player* player = NULL;
Ball* ball = NULL;
Player* bot = NULL;
bool moveUp = false;
bool moveDown = false;



bool initSDL() {


	bool success = true;
	if (TTF_Init() == -1) {
		fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
		success = false;
	}
	else {
		font = TTF_OpenFont("font.otf", 24); 
		if (!font) {
			fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
			success = false;
		}
		else {
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				printf("Failed to initialize! SDL_Error %s\n", SDL_GetError());
				success = false;
			}
			else {
				window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
				if (window == NULL) {
					printf("Failed to create the window! SDL_Error: %s\n", SDL_GetError());
					success = false;
				}
				else {
					renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
					if (!renderer) {
						printf("Failed to create the renderer! SDL_Error: % s\n", SDL_GetError());
						success = false;
					}
				}
			}
		}
	}
	return success;
}

void resetGame() {
	player->reset();
	ball->reset();
	bot->reset();
}
void initGame() {
	player = new Player(10, 720 / 2, 10, 100);
	bot = new Player(1270, 720 / 2, 10, 100);
	ball = new Ball();
}
void checkGoal() {
	SDL_Rect bRect = ball->getBall();
	if (bRect.x <= 2) {
		bot->addScore();
		resetGame();
	}
	else if (bRect.x >= 1278) {
		player->addScore();
		resetGame();
	}
}
void movePlayer() {
	if (moveUp) player->decrementY();
	else if (moveDown) player->incrementY();

}

void moveBot() {
	SDL_Rect bRect = ball->getBall();
	SDL_Rect botRect = bot->getPlayer();

	int ballY = bRect.y;
	int ballX = bRect.x;
	int botY = botRect.y;
	int botX = botRect.x;
	float error = (rand() % 20 - 10) / 10.0f;  // Random error between -1 and 1

	if (ballY > botY) bot->incrementY(error);
	if (ballY < botY) bot->decrementY(error);

}

void close()
{

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;


	SDL_Quit();
}
void checkPlayerCollision() {
	SDL_Rect bRect = ball->getBall();
	SDL_Rect pRect = player->getPlayer();

	int playerStart = pRect.y;
	int playerEnd = pRect.y + pRect.h;
	int playerSurface = pRect.x + pRect.w + 2;
	int playerHalf = pRect.y + pRect.h / 2;
	if((bRect.y >= playerStart && bRect.y <= playerEnd) && bRect.x == playerSurface) {
		ball->reverseDir();
		if (bRect.y > playerHalf) ball->setYDir(1);
		else if (bRect.y < playerHalf) ball->setYDir(-1);
		else ball->setYDir(0);
	}


}
void checkBotCollision() {
	SDL_Rect bRect = ball->getBall();
	SDL_Rect pRect = bot->getPlayer();

	int playerStart = pRect.y;
	int playerEnd = pRect.y + pRect.h;
	int playerSurface = pRect.x - pRect.w + 2;
	int playerHalf = pRect.y + pRect.h / 2;
	if ((bRect.y >= playerStart && bRect.y <= playerEnd) && bRect.x == playerSurface) {
		ball->reverseDir();
		if (bRect.y > playerHalf) ball->setYDir(1);
		else if (bRect.y < playerHalf) ball->setYDir(-1);
		else ball->setYDir(0);
	}


}


void checkWallsCollision() {
	SDL_Rect bRect = ball->getBall();
	int wallHalf = 1280 / 2;
	if( bRect.y <= 10 || bRect.y >= 710) {
		ball->reverseYDir();
	}
}
void checkCollision() {
	checkPlayerCollision();
	checkBotCollision();
	checkGoal();
	checkWallsCollision();
}
void renderWin() {
	SDL_Color color = { 255, 255, 255 };

	SDL_Surface* winSurface = TTF_RenderText_Solid(font, "You win!", color);

	SDL_Texture* winTexture = SDL_CreateTextureFromSurface(renderer, winSurface);
	SDL_Rect centered = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 250, 250 };

	SDL_RenderCopy(renderer, winTexture, NULL, &centered);

}

void renderLose() {
	SDL_Color color = { 255, 255, 255 };

	SDL_Surface* loseSurface = TTF_RenderText_Solid(font, "You lose!", color);

	SDL_Texture* loseTexture = SDL_CreateTextureFromSurface(renderer, loseSurface);
	SDL_Rect centered = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 250, 250 };

	SDL_RenderCopy(renderer, loseTexture, NULL, &centered);

}
void renderScore() {
	string playerScore = to_string(player->getScore());
	string botScore = to_string(bot->getScore());

	SDL_Color color = { 255, 255, 255 };

	SDL_Surface* playerSurface = TTF_RenderText_Solid(font, playerScore.c_str(), color);
	SDL_Surface* botSurface = TTF_RenderText_Solid(font, botScore.c_str(), color);

	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_Texture* botTexture = SDL_CreateTextureFromSurface(renderer, botSurface);

	int pX = 1280 / 4;
	int botX = 1280 / 2 + (1280 / 4) ;
	SDL_Rect dstPlayerRect = { pX, 100, 100, 100 };
	SDL_Rect dstBotRect = { botX, 100, 100, 100 };  

	SDL_RenderCopy(renderer, playerTexture, NULL, &dstPlayerRect);
	SDL_RenderCopy(renderer, botTexture, NULL, &dstBotRect);


		
}
int main(int argc, char* argv[]) {
	bool quit = false;
	SDL_Event e;
	
	if (!initSDL()) {
		printf("Failed to initialize!\n");
	}
	else {
		initGame();
		if (player == NULL || bot == NULL)  {
			printf("Failed to init player! \n");
		}
		else {
			if (ball == NULL) {
				printf("Failed to init ball! \n");
			}
			else {
				while (!quit) {
					frameStart = SDL_GetTicks(); // Get the number of milliseconds since SDL was initialized

					while (SDL_PollEvent(&e) != 0) {
						if (e.type == SDL_QUIT) {
							quit = true;
						}
						else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
							switch (e.key.keysym.sym) {
							case SDLK_UP:
								moveUp = true;
								break;
							case SDLK_DOWN:
								moveDown = true;
								break;
							default:
								break;
							}
						}
						else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
							switch (e.key.keysym.sym) {

							case SDLK_UP:
								moveUp = false;
								break;
							case SDLK_DOWN:
								moveDown = false;
								break;
							default:
								break;
							}
						}


					}
					movePlayer();
					moveBot();
					checkCollision();
					ball->move();
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderClear(renderer);

					player->render(renderer);
					ball->render(renderer);
					bot->render(renderer);

					renderScore();
					if (player->getScore() >= 5 || bot->getScore() >= 5) {
						if (player->getScore() >= 5) {

							while (1) {
								SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

								SDL_RenderClear(renderer);
								renderWin();
								SDL_RenderPresent(renderer);
								while (SDL_PollEvent(&e) != 0) {
									if (e.type == SDL_QUIT) {
										quit = true;
									}
								}
							}
						}
						else {
							while (1) {
								SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

								SDL_RenderClear(renderer);
								renderLose();
								SDL_RenderPresent(renderer);
								while (SDL_PollEvent(&e) != 0) {
									if (e.type == SDL_QUIT) {
										quit = true;
									}
								}
							}
						}
						
					}

					SDL_RenderPresent(renderer);

					frameTime = SDL_GetTicks() - frameStart; // Calculate frame duration

					// If frame finished early, wait to maintain frame rate
					if (frameDelay > frameTime) {
						SDL_Delay(frameDelay - frameTime); // Add delay to cap frame rate
					}
				}

			}
		}
	}
	close();
	return 0;
}