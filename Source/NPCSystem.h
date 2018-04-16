#pragma once
#include "Entity.h"
#include "CollisionSystem.h"
#include "Animator.h"
#include "AIComponent.h"
class NPCSystem {
private:
	NPCSystem();
public:
	static Entity* createNPC(int posX, int posY, Globals::Layers layer, SDL_Texture* texture, std::string spriteName, Abilities* abilities = NULL, Talkable* talkable = NULL);
};