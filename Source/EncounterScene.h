#pragma once
#include "Scene.h"
class EncounterScene : public Scene {
public:
	explicit EncounterScene(SDL_Renderer* renderer, Graphics* graphics);
	void update(float deltaTime);
	~EncounterScene();
private:
	SDL_Texture* tree;
	SDL_Texture* treeNoLeaves;
	std::vector<Entity*> trees;
};