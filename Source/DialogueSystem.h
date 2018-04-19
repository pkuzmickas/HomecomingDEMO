#pragma once
#include "Entity.h"
#include <fstream>
#include <iostream>
#include <rapidxml.hpp>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
using namespace rapidxml;
class DialogueSystem {
public:
	struct Actor {
		string imgPath;
		string name;
	};
	static void loadDialogues(std::string filePath);
private:
	DialogueSystem();
	static unordered_map<string, vector<Talkable::Dialogue>> dialogues;
	static unordered_map<string, Actor> actors;
};