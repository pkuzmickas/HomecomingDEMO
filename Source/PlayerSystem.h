#pragma once
#include "Movement.h"
#include "PlayerStats.h"
#include "PlayerInput.h"
#include "PlayerMovement.h"
#include "PlayerAnimator.h"
#include "DialogueSystem.h"
#include "PlayerAbilities.h"

class PlayerSystem {
public:
	static Entity* createPlayer(int globalPosX, int globalPosY, SDL_Texture* texture, SDL_Renderer* renderer, Graphics* graphics, PlayerAnimator::LookDirection lookDirection = PlayerAnimator::LookDirection::RIGHT);
	static void disableMovement();
	static void enableMovement();
	static Entity* getPlayer();
	static void resetPlayer();
private:
	PlayerSystem();
	static Entity* player;

};