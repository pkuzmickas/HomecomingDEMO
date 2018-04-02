#pragma once
#include "Entity.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <map>
class Graphics {
public:
	void addToDraw(Entity* entity); // for game objects
	void addMap(std::vector<std::vector<std::vector<Entity*>>> mapMatrix);
	Graphics(SDL_Renderer* renderer);
	void render();
private:
	// Map is drawn in a separate way to increase performance
	std::vector<std::vector<std::map<Globals::Layers, Entity*>>> mapDrawQueue; // [ROW] [COL] [LAYER]
	std::vector<std::vector<Entity*>> objectDrawQueue; // [LAYER] [ENTITY]
	SDL_Renderer* renderer;
	void renderObjects();
	void renderMap();
	bool debug = true;
};