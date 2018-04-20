#pragma once
#include <fstream>
#include <iostream>
#include <rapidxml.hpp>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include "Entity.h"

using namespace std;
using namespace rapidxml;
class DialogueSystem {
public:
	struct Actor {
		string imgPath;
		string name;
	};
	static void setup(SDL_Texture* dialogueBox);
	static void loadDialogues(std::string filePath);
	static unordered_map<string, Talkable::Dialogue> dialogues;
	static unordered_map<string, Actor> actors;
	// TODO create an OpenDialogue(all data and ^) then CloseDialogue or sth :)
	static void openDialogueBox();
	static void closeDialogueBox();
	static void update(float deltaTime);
	static void cleanup();
	static bool isOpen() { return open; }
private:
	DialogueSystem();
	static Entity* createDialogueBox();
	static bool setuped;
	static bool open;
	static SDL_Texture* dialogueBoxIMG;

	

	 
};