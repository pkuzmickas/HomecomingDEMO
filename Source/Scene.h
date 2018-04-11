#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "PlayerSystem.h"
#include "SceneDesignSystem.h"

class Scene {
public:
	~Scene();
	virtual void update(float deltaTime);
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
	

};