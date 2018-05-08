#pragma once
#include "DialogueSystem.h"
#include "CollisionSystem.h"
#include "Animator.h"
#include "AISoldier.h"
class NPCSystem {
private:
	NPCSystem();
public:
	static Entity* createNPC(int posX, int posY, int width, int height,Globals::Layers layer, SDL_Texture* texture, std::string spriteName, std::string* speakerID = NULL);
	static Entity* createSoldier(int posX, int posY, int width, int height, Globals::Layers layer, SDL_Texture* texture, std::string spriteName, SDL_Renderer* renderer, Graphics* graphics, int hp, std::string* speakerID = NULL);
	static Entity* createBoss(int posX, int posY, int width, int height, Globals::Layers layer, SDL_Texture* texture, std::string spriteName, SDL_Renderer* renderer, Graphics* graphics, std::string* speakerID = NULL);

	//Remove graphics and sdl renderer from constructor when refactoring into an "abilities" system
};