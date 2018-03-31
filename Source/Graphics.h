#pragma once
#include "Entity.h"
#include <vector>
#include <iostream>
class Graphics {
public:
	void addToDraw(Entity* entity); // for game objects
	void addMap(std::vector<std::vector<std::vector<Entity*>>> mapMatrix);
	Graphics(SDL_Renderer* renderer);
	void render();
private:
	std::vector<std::vector<Entity*>> mapDrawQueue;
	std::vector<std::vector<Entity*>> objectDrawQueue; // [LAYER] [ENTITY]
	SDL_Renderer* renderer;
	void renderDrawQueue(std::vector<std::vector<Entity*>> drawQueue);
	bool debug = true;
};