#include "Circle.h"
#include <iostream>
Circle::Circle(int x, int y) :x(x), y(y){
	ticks = SDL_GetTicks();
	movev = ticks;
	speed = 200;
	stepX = 0;
	stepY = 0;
}

void Circle::move(bool* held){
	float step = speed * (movev - ticks) / 1000;
	if (held[SDLK_SPACE]) step *= 2;
	ticks = movev;
	movev = SDL_GetTicks();
	if (held[SDLK_w]) y -= step;
	if (held[SDLK_s]) y += step;
	if (held[SDLK_a]) x -= step;
	if (held[SDLK_d]) x += step;
	if (x < 0) x = 0;
	if (x + 30 > 640) x = 640 -30;
	if (y < 0) y = 0;
	if (y + 30 > 480) y = 480- 30;
}

void Circle::render(SDL_Renderer* renderer){
	SDL_Rect rect;	
	rect.x = (int)x;
	rect.y = (int)y;
	rect.w = 30;
	rect.h = 30;
	SDL_SetRenderDrawColor(renderer, 20, 40, 50, 0);
	SDL_RenderFillRect(renderer, &rect);
}