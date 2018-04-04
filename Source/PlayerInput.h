#pragma once
#include "Entity.h"
#include "PlayerStats.h"
#include "PlayerMovement.h"
#include <vector>
class PlayerInput : public Input {
private:
	PlayerMovement* playerMovement;
	PlayerStats* playerStats;
public:
	PlayerInput(Entity* owner);
	void update(float deltaTime);
};