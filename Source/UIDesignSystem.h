#pragma once
#include "Entity.h"
#include "Graphics.h"
#include <unordered_map>
class UIDesignSystem {
private:
	UIDesignSystem();
	static SDL_Renderer* renderer;
	static Graphics* graphics;
	static SDL_Texture* healthBarFull;
	static SDL_Texture* healthBarEmpty;
	static unordered_map<Entity*, Entity*> emptyBars; // gameObject to its healthbar
	static unordered_map<Entity*, Entity*> fullBars; // gameObject to its healthbar
	static int healthbarHeight;
	static int healthbarWidth;
public:
	static void setup(SDL_Renderer* renderer, Graphics* graphics);
	static void showHealth(Entity * entity);
	static void update(float deltaTime);
	static bool isHealthShowing(Entity* entity);
	static void cleanup(); // to be called when exiting the game
};