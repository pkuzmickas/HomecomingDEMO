#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Constants.h"
#include <SDL_ttf.h>
#include <sstream>


class Director {
public:
	Director(SDL_Renderer* renderer);
	~Director();
	void startGame();
	
private:
	bool gameRunning = true;
	float deltaTime; // time since last frame
	SDL_Renderer* renderer;

	void getInput();
	void update();
	void draw();
};

