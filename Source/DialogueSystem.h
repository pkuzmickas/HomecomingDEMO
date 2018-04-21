#pragma once
#include <fstream>
#include <iostream>
#include <rapidxml.hpp>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include "Graphics.h"
#include "SceneDesignSystem.h"

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
	static void openDialogueBox(string dialogueID);
	static void closeDialogueBox();
	static void update(float deltaTime);
	// To be called when closing the game
	static void cleanup();
	static bool isOpen() { return open; }
	static int dialogueSpeed;
private:
	DialogueSystem();
	static void getInput();
	// Switches to the next dialogue text
	static void switchDialogue();
	static void initDialogue();
	// Deletes everything except the dialogue box entity
	static void cleanupDialogue();
	static Entity* createDialogueBox();
	static Entity* createSpeakerIMG(std::string speakerID);
	static bool setuped;
	static bool open;
	static bool animating;
	static SDL_Texture* dialogueBoxTexture;
	static Graphics* graphics;
	static Entity* dialogueBox;
	static Entity* speakerIMG;
	static Entity* speakerName;
	static Entity* dialogueText;
	static Transform* dialogueBoxTransform;
	static Transform* speakerIMGTransform;
	static Transform* speakerNameTransform;
	static Transform* dialogueTextTransform;
	static SDL_Texture* speakerIMGTexture;
	static Drawable* dialogueTextDrawable;
	static SDL_Renderer* renderer;
	static int textHeight;
	static int textWidth;
	static int curLetter;
	static int lastTimeChecked;
	static string curDialogueID;
	static int curDialogueTextIndex;
	static int fontSize;
	 
};