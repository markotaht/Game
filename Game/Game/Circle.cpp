#include "Circle.h"
#include <iostream>
Circle::Circle(int x, int y):x(x),y(y){
	ticks = SDL_GetTicks();
	movev = ticks;
	speed = 200;
	stepX = 1;
	stepY = 1;
}

void Circle::move(){
	float step = speed * (movev - ticks) / 1000;
	ticks = movev;
	movev = SDL_GetTicks();
	x += stepX* step;
	y += stepY *step;

	if (x < 0 || x+30 > 640) stepX *= -1;
	if (y < 0 || y+30 > 480) stepY *= -1;
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