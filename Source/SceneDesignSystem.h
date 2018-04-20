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
	static Entity* curText;
	static SDL_Texture* curTextTexture;
public:
	static TTF_Font* textFont;
	static Entity* createTree(int posX, int posY, Globals::Layers layer, SDL_Texture* texture);
	static Entity* createRect(int posX, int posY, int width, int height, Globals::Layers layer, bool fill);
	static Entity* createText(std::string text, int posX, int posY, int fontSize, SDL_Color color, Globals::Layers layer, SDL_Renderer* renderer);
	static void cleanupText();
};