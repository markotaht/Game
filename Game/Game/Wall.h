#ifndef WALL_H
#define WALL_H

#include <SDL.h>
#include <set>
typedef struct line{
	SDL_Point a;
	SDL_Point b;
}LINE;

class Wall{
private:
	SDL_Rect rect;
	int width = 30;
	int height = 30;
public:
	Wall(int x, int y);
	void render(SDL_Renderer* renderer);
	void getPoints(std::set<SDL_Point*>* points, std::set<LINE*>* lines);
	inline SDL_Rect getRect(){ return rect; }
};
#endif