#pragma once
#include "DialogueSystem.h"
#include "CollisionSystem.h"
#include "Animator.h"
#include "AIComponent.h"
class NPCSystem {
private:
	NPCSystem();
public:
	static Entity* createNPC(int posX, int posY, int width, int height,Globals::Layers layer, SDL_Texture* texture, std::string spriteName, std::string* speakerID = NULL, Abilities* abilities = NULL);
};