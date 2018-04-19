#pragma once
#include <fstream>
#include <iostream>
#include <rapidxml.hpp>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include "MapSystem.h"
//#include "Globals.h"
using namespace std;
using namespace rapidxml;
class DialogueSystem {
public:
	struct Actor {
		string imgPath;
		string name;
	};
	static void loadDialogues(std::string filePath);
	static unordered_map<string, vector<Talkable::Dialogue>> dialogues;
	static unordered_map<string, Actor> actors;
	static Entity* createDialogueBox(SDL_Texture* dialogueIMG);
	// TODO create an OpenDialogue(all data and ^) then CloseDialogue or sth :)
	static Entity* closeDialogueBox();
	static void update(float deltaTime);
private:
	DialogueSystem();
	 
};