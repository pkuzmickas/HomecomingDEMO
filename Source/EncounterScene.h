#pragma once
#include "Scene.h"
class EncounterScene : public Scene {
public:
	explicit EncounterScene(SDL_Renderer* renderer, Graphics* graphics);
	void update(float deltaTime);
	~EncounterScene();
private:
	void setup();
	SDL_Texture* tree;
	SDL_Texture* treeNoLeaves;
	SDL_Texture* oldman;
	SDL_Texture* soldier;

	std::vector<Entity*> entities;
};