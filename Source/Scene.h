#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "PlayerSystem.h"
#include "SceneDesignSystem.h"
#include "NPCSystem.h"
#include "DialogueSystem.h"
#include "UIDesignSystem.h"

class Scene {
public:
	virtual ~Scene();
	virtual void update(float deltaTime);
	bool isGameOver() { return gameOver; }
	void loadAction(std::string actionName) { curAction = actionName; }
protected:
	SDL_Renderer* renderer;
	Graphics* graphics;
	Entity* player;
	
	explicit Scene(SDL_Renderer* renderer, Graphics* graphics);
	
	void createPlayer(int globalPosX, int globalPosY, PlayerAnimator::LookDirection lookDirection = PlayerAnimator::LookDirection::RIGHT); // Centers the camera as well

	std::string curAction;
	
	// For the waiting ACTION
	void wait(float waitSeconds, std::string nextAction);
	float waitSeconds;
	float waitTimePassed;
	std::string nextAction;

	bool gameOver = false;
	SDL_Texture* diedIMG;
	void showLoseScreen();

	

};