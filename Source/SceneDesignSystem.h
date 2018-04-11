#pragma once
#include "Globals.h"
#include "Entity.h"
#include <string>
#include "Collider.h"
#include "Animator.h"
class SceneDesignSystem {
private:
	SceneDesignSystem();
	static int treesSpawned;
public:
	static Entity* createTree(int posX, int posY, Globals::Layers layer, SDL_Texture* texture);
};