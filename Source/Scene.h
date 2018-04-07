#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "PlayerSystem.h"


class Scene {
public:
	~Scene();
	void update(float deltaTime);
protected:
	SDL_Renderer* renderer;
	Graphics* graphics;
	Entity* player;
	
	explicit Scene(SDL_Renderer* renderer, Graphics* graphics);
	
	void createPlayer(int globalPosX, int globalPosY); // Centers the camera as well

	
	

};