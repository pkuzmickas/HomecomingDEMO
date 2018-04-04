#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Graphics.h"
#include "Map.h"
#include "Movement.h"
#include "PlayerStats.h"
#include "PlayerInput.h"
#include "PlayerMovement.h"

class Scene {
public:
	~Scene();
	void update(float deltaTime);
protected:
	SDL_Renderer* renderer;
	Graphics* graphics;
	Map* map;
	Entity* player;
	
	explicit Scene(SDL_Renderer* renderer, Graphics* graphics);
	
	void createPlayer(int globalPosX, int globalPosY); // Centers the camera as well

	
	

};