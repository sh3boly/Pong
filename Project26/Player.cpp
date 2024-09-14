#include "Player.h"



void Player::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderFillRect(renderer, &rect);  // Draw the paddle
}

void Player::addScore()
{
    score += 1;
}

void Player::reset()
{
    rect.y = 720 / 2;
}

int Player::getScore()
{
    return score;
}

SDL_Rect Player::getPlayer()
{
    return rect;
}

pair<int, int> Player::getPos()
{
    pair<int, int> pos;
    pos.first = rect.x;
    pos.second = rect.y;
    return pos;
}

void Player::decrementY(float error)
{

    if(!(rect.y <= 10)) rect.y = rect.y - (4 * error);
}

void Player::incrementY(float error)
{
    if(!(rect.y >= 610)) rect.y =  rect.y + (4 * error);
}
