#pragma once
#include "Globals.h"
#include "Entity.h"
#include <string>
#include "Animator.h"
#include "CollisionSystem.h"
class SceneDesignSystem {
private:
	SceneDesignSystem();
	static int treesSpawned;
public:
	static Entity* createTree(int posX, int posY, Globals::Layers layer, SDL_Texture* texture);
	static Entity* createRect(int posX, int posY, int width, int height, Globals::Layers layer, bool fill);
};