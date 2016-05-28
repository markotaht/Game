#include "Wall.h"

Wall::Wall(int x, int y){
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}

void Wall::render(SDL_Renderer* renderer){
	SDL_SetRenderDrawColor(renderer, 150, 40, 50, 0);
	SDL_RenderFillRect(renderer, &rect);
}

void Wall::getPoints(std::set<SDL_Point*>* points, std::set<LINE*>* lines){
	SDL_Point* p = new SDL_Point{ rect.x, rect.y };
	SDL_Point* p2 = new SDL_Point{ rect.x, rect.y + rect.h };
	SDL_Point* p4 = new SDL_Point{ rect.x + rect.w, rect.y };
	SDL_Point* p3 = new SDL_Point{ rect.x + rect.w, rect.y + rect.h };
	LINE* l = new LINE{ *p, *p2 };
	LINE* l2 = new LINE{ *p2, *p3 };
	LINE* l3 = new LINE{ *p3, *p4 };
	LINE* l4 = new LINE{ *p4, *p };

	lines->insert(l);
	lines->insert(l2);
	lines->insert(l3);
	lines->insert(l4);

	points->insert(p);
	points->insert(p2);
	points->insert(p3);
	points->insert(p4);
}