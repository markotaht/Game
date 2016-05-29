#include "Circle.h"

Circle::Circle(float x, float y, float radius) : x(x), y(y), radius(radius){}

LINE* Circle::getVisiblePoints(float cx, float cy){
	float hyp = sqrtf((x - cx)*(x - cx) + (y - cy)*(y - cy));
	float angle = asinf(radius / hyp);

	float l = sqrtf(hyp*hyp - radius*radius);

	SDL_Point v{ x - cx, y - cy };
	float ratio = hyp / l;

	float cs = cos(angle);
	float sn = sin(angle);

	float px = v.x*cs - v.y * sn ;
	float py = v.x * sn - v.y*cs ;

	px *= ratio;
	py *= -ratio;

	float cs2 = cos(angle);
	float sn2 = sin(-angle);

	float px2 = v.x*cs2 - v.y * sn2 ;
	float py2 = v.x * sn2 - v.y*cs2 ;

	px2 *= ratio;
	py2 *= -ratio;

	SDL_Point a{ px2, py };
	SDL_Point b{ px, py2 };

	return new LINE{ a, b };
}