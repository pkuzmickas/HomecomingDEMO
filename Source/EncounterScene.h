#pragma once
#include "Scene.h"
class EncounterScene : public Scene {
public:
	explicit EncounterScene(SDL_Renderer* renderer, Graphics* graphics);
	void update(float deltaTime);
	~EncounterScene();
private:
	void setup();
	void preFightScenario(float deltaTime);
	SDL_Texture* tree;
	SDL_Texture* treeNoLeaves;
	SDL_Texture* oldman;
	SDL_Texture* soldier;
	/*Entity* oldmanEntity;
	Entity* soldierEntity;
	Entity* soldier2Entity;*/
	AIComponent* oldmanAI;
	AIComponent* soldierAI;
	AIComponent* soldier2AI;
	Transform* blackBox1T;
	Transform* blackBox2T;
	std::vector<Entity*> entities;
	std::string introText = "I WILL SAVE EVERYONE...";
	int textProgress = 1;
	TTF_Font* textFont;
	SDL_Texture* textTexture;
	Entity* textEntity;
};