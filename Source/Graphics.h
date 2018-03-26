#pragma once
#include "Entity.h"
#include <vector>
#include <iostream>
class Graphics {
public:
	void addToDraw(Entity* entity);
	Graphics(SDL_Renderer* renderer);
	void render();
private:
	std::vector<std::vector<Entity*>> drawQueue;
	SDL_Renderer* renderer;
};