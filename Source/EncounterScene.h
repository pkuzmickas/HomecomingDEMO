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
	SDL_Texture* oldman;
	SDL_Texture* soldier;
	AIBoss* oldmanAI;
	AISoldier* soldierAI;
	AISoldier* soldier2AI;
	Transform* blackBox1T;
	Transform* blackBox2T;
	std::vector<Entity*> entities;
	std::string introText = "I WILL COME BACK FOR YOU";
	int textProgress = 1;
	SDL_Texture* textTexture;
	Entity* textEntity;

	vector<Entity*> removedEntities;
	vector<Entity*> affectedEntities;

	Entity* boundary1 = NULL;
	Entity* boundary2 = NULL;
};