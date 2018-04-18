#pragma once
#include "Entity.h"
#include "Collider.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <map>
#include "MapSystem.h"
class Graphics {
public:
	void addToDraw(Entity* entity); // for game objects
	bool removeFromDraw(Entity* entity);
	void addMap(std::vector<std::vector<std::vector<Entity*>>> mapMatrix);
	Graphics(SDL_Renderer* renderer);
	void render();
	bool debug = false;
private:
	struct Sprite {
		Transform* transform;
		Drawable* drawable;
		Entity* entity;
	};

	// Map is drawn in a separate way to increase performance
	std::vector<std::vector<Sprite>> mapDrawQueue; // [LAYER] [ENTITY]
	std::vector<std::vector<Sprite>> objectDrawQueue; // [LAYER] [ENTITY]
	std::vector<std::vector<std::vector<Sprite>>> mapMatrixWSprites;
	SDL_Renderer* renderer;
	int mapRows = 0;
	int mapCols = 0;
	void draw(Sprite sprite);
};