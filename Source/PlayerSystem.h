#pragma once
#include "Entity.h"
#include "Movement.h"
#include "PlayerStats.h"
#include "PlayerInput.h"
#include "PlayerMovement.h"
#include "Map.h"
#include "Graphics.h"
#include "Animator.h"

class PlayerSystem {
public:
	PlayerSystem(SDL_Renderer* renderer);
	Entity* createPlayer(int globalPosX, int globalPosY);
private:
	Entity* player;
	SDL_Renderer* renderer;
};