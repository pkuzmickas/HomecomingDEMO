#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <sstream>
#include "Graphics.h"
#include "EncounterScene.h"
class Director {
public:
	Director(SDL_Renderer* renderer);
	~Director();
	void startGame();
	
private:
	bool gameRunning = true;
	float deltaTime; // time since last frame
	SDL_Renderer* renderer;
	Graphics* graphics;
	void getInput();
	void update();
	Scene* curScene;
};

