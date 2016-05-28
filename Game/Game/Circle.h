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
	void move(bool *held);
	void render(SDL_Renderer* renderer);
	inline float getCenterX(){ return x + 15; }
	inline float getCenterY(){ return y + 15; }
};
#endif