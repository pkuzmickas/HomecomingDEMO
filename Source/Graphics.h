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
	struct Sprite {
		Transform* transform;
		Drawable* drawable;
	};

	// Map is drawn in a separate way to increase performance
	std::vector<std::vector<Sprite>> mapDrawQueue; // [LAYER] [ENTITY]
	std::vector<std::vector<Sprite>> objectDrawQueue; // [LAYER] [ENTITY]
	std::vector<std::vector<std::vector<Sprite>>> mapMatrixWSprites;
	SDL_Renderer* renderer;
	bool debug = true;
	int mapRows = 0;
	int mapCols = 0;
	void draw(Sprite sprite);
};