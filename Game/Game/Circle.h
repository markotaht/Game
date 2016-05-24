#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL.h>
class Circle{
private:
	float x, y;
	int stepX;
	int stepY;
	int speed;
	Uint32 ticks;
	Uint32 movev;
public:
	Circle(int x, int y);
	void move();
	void render(SDL_Renderer* renderer);
};
#endif