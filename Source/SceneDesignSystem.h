#pragma once
#include "Globals.h"
#include "Entity.h"
#include <string>
#include "Animator.h"
#include <SDL_ttf.h>
#include "CollisionSystem.h"
class SceneDesignSystem {
private:
	SceneDesignSystem();
	static int treesSpawned;
public:
	static void checkFontSizeOnText(std::string text, int fontSize, int* width, int* height);
	static Entity* createTree(int posX, int posY, Globals::Layers layer, SDL_Texture* texture, SDL_Rect* colOffset = NULL);
	static Entity* createRect(int posX, int posY, int width, int height, Globals::Layers layer, bool fill);
	static Entity* createText(std::string text, int posX, int posY, int fontSize, SDL_Color color, Globals::Layers layer, SDL_Renderer* renderer);
	static void cleanupText(Entity* text);
};