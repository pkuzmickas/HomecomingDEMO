#pragma once
#include "Entity.h"
#include "PlayerStats.h"
#include <vector>
class PlayerInput : public Input {
private:
	Movement* playerMovement;
	PlayerStats* playerStats;
public:
	PlayerInput(Entity* owner);
	void update(float deltaTime);
};