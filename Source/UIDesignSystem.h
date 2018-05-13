#pragma once
#include "Entity.h"
#include "Graphics.h"
#include "Animator.h"
#include "PlayerStats.h"
#include <unordered_map>
class UIDesignSystem {
private:
	UIDesignSystem();
	static SDL_Renderer* renderer;
	static Graphics* graphics;
	static SDL_Texture* healthBarFull;
	static SDL_Texture* healthBarEmpty;
	static SDL_Texture* bloodshotIMG;
	static SDL_Texture* playerHealthBarFull;
	static SDL_Texture* playerHealthBarEmpty;
	static SDL_Texture* attackUI;
	static unordered_map<Entity*, Entity*> emptyBars; // gameObject to its healthbar
	static unordered_map<Entity*, Entity*> fullBars; // gameObject to its healthbar
	static vector<Entity*> bloodShots;
	static int healthbarHeight;
	static int healthbarWidth;
	static int playerHealthbarWidth;
	static int playerHealthbarHeight;
	static Entity* playerEmptyBar;
	static Entity* playerFullBar;
	static Entity* playerEntity;
	static vector<Entity*> attackIcons;
	static bool showingPlayerUI;
public:
	static void setup(SDL_Renderer* renderer, Graphics* graphics);
	static void showHealth(Entity * entity);
	static void hideHealth(Entity * entity);
	static void showPlayerUI(Entity* player);
	static void hidePlayerUI();
	static void activateAttack(int id);
	static void deactivateAttack(int id);
	static void update(float deltaTime);
	static bool isHealthShowing(Entity* entity);
	static void createBloodshot(Entity* entity);
	static void cleanup(); // to be called when exiting the game
};