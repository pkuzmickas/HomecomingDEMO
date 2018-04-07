#pragma once
#include "Entity.h"
#include "Movement.h"
#include "PlayerStats.h"
#include "PlayerInput.h"
#include "PlayerMovement.h"
#include "PlayerAnimator.h"
#include "MapSystem.h"
#include "Graphics.h"

class PlayerSystem {
public:
	static Entity* createPlayer(int globalPosX, int globalPosY, SDL_Texture* texture);
private:
	PlayerSystem();

};