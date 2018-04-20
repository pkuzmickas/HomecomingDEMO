#pragma once
#include <fstream>
#include <iostream>
#include <rapidxml.hpp>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include "Entity.h"
#include "Graphics.h"

using namespace std;
using namespace rapidxml;
class DialogueSystem {
public:
	struct Actor {
		string imgPath;
		string name;
	};
	static void setup(Graphics* graphics, SDL_Renderer* renderer);
	static void loadDialogues(std::string filePath);
	static unordered_map<string, Talkable::Dialogue> dialogues;
	static unordered_map<string, Actor> actors;
	// TODO create an OpenDialogue(all data and ^) then CloseDialogue or sth :)
	static void openDialogueBox(string dialogueID);
	static void closeDialogueBox();
	static void update(float deltaTime);
	static void cleanup();
	static bool isOpen() { return open; }
private:
	DialogueSystem();
	static Entity* createDialogueBox();
	static Entity* createSpeakerIMG(std::string speakerID);
	static bool setuped;
	static bool open;
	static SDL_Texture* dialogueBoxTexture;
	static Graphics* graphics;
	static Entity* dialogueBox;
	static Entity* speakerIMG;
	static Transform* dialogueBoxTransform;
	static Transform* speakerIMGTransform;
	static SDL_Texture* speakerIMGTexture;
	static SDL_Renderer* renderer;
	

	 
};