#ifndef CIRCLE_H
#define CIRCLE_H

#include "Wall.h"
#include <math.h>
#include <SDL.h>

class Circle{
private:
	float x;
	float y;
	float radius;
public:
	Circle(float x, float y, float radius);
	LINE* getVisiblePoints(float x, float y);
};
#endif