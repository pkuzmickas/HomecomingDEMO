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

class MapSystem {
public:
	
	static void createMap(SDL_Renderer* renderer, std::string path);
	static std::vector<std::vector<std::vector<Entity*>>> getMap() { return mapMatrix; }
	static int getWidth() { return levelWidth; }
	static int getHeight() { return levelHeight; }
	static void deleteMap();
private:
	MapSystem();
	static bool readFile(std::string path, std::vector<char> &buffer);
	static std::vector<char> MapSystem::loadMap(std::string path);
	static int levelWidth; //in pixels
	static int levelHeight; //in pixels
	static std::vector<std::vector<std::vector<Entity*>>> mapMatrix; // [ROW] [COLUMN] [WHICH ENTITY in that pos]
	

};