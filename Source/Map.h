#pragma once
#include "rapidxml.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include "Globals.h"
#include "Entity.h"

class Map {
public:
	bool readFile(std::string path, std::vector<char> &buffer);
	std::vector<char> Map::loadMap(std::string path);
	Map(SDL_Renderer* renderer, std::string path);
	std::vector<std::vector<std::vector<Entity*>>> getMap() { return mapMatrix; }
	~Map();
private:
	int width; //how many tiles are in the X axis
	int height; //how many tiles are in the Y axis
	int levelWidth; //in pixels
	int levelHeight; //in pixels
	int tilecount;
	int columns;
	std::string spriteSheetPath;
	int ssWidth;
	int ssHeight;
	rapidxml::xml_node<>* root_node;
	rapidxml::xml_document<> doc;
	SDL_Renderer* gRenderer;
	SDL_Texture* spriteSheet;
	std::vector<std::vector<std::vector<Entity*>>> mapMatrix; // [ROW] [COLUMN] [WHICH ENTITY in that pos]

};