/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <math.h>
#include "Circle.h"
#include "Wall.h"

#define PI 3.14159265

typedef struct point{
	float x;
	float y;
	float angle;
	float T;
}Point;

typedef struct ray{
	float x;
	float y;
	float angle;
}RAY;
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

Point* getIntesection(RAY ray, LINE segment){
	float r_px = ray.x;
	float r_py = ray.y;
	float r_dx = cos(ray.angle);
	float r_dy = sin(ray.angle);
	// SEGMENT in parametric: Point + Delta*T2
	float s_px = segment.a.x;
	float s_py = segment.a.y;
	float s_dx = segment.b.x - segment.a.x;
	float s_dy = segment.b.y - segment.a.y;

	// Are they parallel? If so, no intersect
	float r_mag = sqrt(r_dx*r_dx + r_dy*r_dy);
	float s_mag = sqrt(s_dx*s_dx + s_dy*s_dy);
	if (r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag){
		// Unit vectors are the same.
		return NULL;
	}

	float T2 = (r_dx*(s_py - r_py) + r_dy*(r_px - s_px)) / (s_dx*r_dy - s_dy*r_dx);
	float T1 = (s_px + s_dx*T2 - r_px) / r_dx;
	// Must be within parametic whatevers for RAY/SEGMENT
	if (T1<0) return NULL;
	if (T2<0 || T2>1) return NULL;

	Point* p = new Point;
	p->x = r_px + r_dx*T1;
	p->y = r_py + r_dy*T1;
	p->T = T1;
	return p;
}

void render_polygon(std::vector<Point*> polygon){
	std::vector<Point*>::iterator it = polygon.begin();
	Point* p = *it;
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	it++;
	Point* t = p;
	for (; it != polygon.end(); it++){
		SDL_RenderDrawLine(gRenderer, t->x, t->y, (*it)->x, (*it)->y);
		t = *it;
	}
	SDL_RenderDrawLine(gRenderer, t->x, t->y, p->x, p->y);
}
struct PointerCompare {
	int operator()(const Point* l, const Point* r) {
		return l->angle < r->angle;
	}
};

void raycast(Circle* c, std::set<SDL_Point*>* points, std::set<LINE*>* lines){
	float centerX = (c->getCenterX());
	float centerY = (c->getCenterY());
	std::vector<Point*> polygon;
	for (std::set<SDL_Point*>::iterator it = points->begin(); it != points->end(); ++it){
		float tempdx = (*it)->x - centerX;
		float tempdy = (*it)->y - centerY;
		float angle = atan2(tempdy, tempdx);
		float angles[3] = {angle-0.0001, angle, angle+0.0001};
		for (int i = 0; i < 3; i++){
			RAY r{ centerX, centerY, angles[i] };

			Point* closest = NULL;
			for (std::set<LINE*>::iterator it2 = lines->begin(); it2 != lines->end(); ++it2){
				Point* D = getIntesection(r, *(*it2));
				if (D == NULL) continue;
				if (closest == NULL || D->T < closest->T) closest = D;
			}
			if (closest == NULL) continue;
			closest->angle = angles[i];

			polygon.push_back(closest);
		}
	}
	std::cout << "algus" << std::endl;
	std::sort(polygon.begin(), polygon.end(), PointerCompare());
	for (std::vector<Point*>::iterator it = polygon.begin(); it != polygon.end(); ++it){
		std::cout << (*it)->angle << std::endl;
	}
	render_polygon(polygon);
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
	/*	if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{*/
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			std::set<LINE*> lines;
			std::set<SDL_Point*> points;
			Circle c(30, 30);
			Wall w(180, 180);
			Wall w2(300, 300);
			Wall w3(400, 100);
			SDL_Rect r{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			w.getPoints(&points, &lines);
			w2.getPoints(&points, &lines);
			w3.getPoints(&points, &lines);
			SDL_Point p{ 0, 0 };
			SDL_Point p2{ 0, SCREEN_HEIGHT };
			SDL_Point p3{ SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_Point p4{ SCREEN_WIDTH, 0 };

			LINE l{ p, p2 };
			LINE l2{ p2, p3 };
			LINE l3{ p3, p4 };
			LINE l4{ p4, p };

			lines.insert(&l);
			lines.insert(&l2);
			lines.insert(&l3);
			lines.insert(&l4);

			points.insert(&p);
			points.insert(&p2);
			points.insert(&p3);
			points.insert(&p4);

			int delay = 1000 / 120;
			bool keysHeld[323] = { false };
			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					if (e.type == SDL_KEYDOWN)
					{
						keysHeld[e.key.keysym.sym] = true;
					}
					if (e.type == SDL_KEYUP)
					{
						keysHeld[e.key.keysym.sym] = false;
					}
				}
				c.move(keysHeld);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				//Clear screen
				SDL_RenderClear(gRenderer);
				//Update screen
				raycast(&c, &points, &lines);
				c.render(gRenderer);
				w.render(gRenderer);
				w2.render(gRenderer);
				w3.render(gRenderer);

				SDL_RenderPresent(gRenderer);
				SDL_Delay(delay);
			}
		//}
	}

	//Free resources and close SDL
	close();

	return 0;
}
