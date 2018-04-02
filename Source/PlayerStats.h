#pragma once
#include "Component.h"

class PlayerStats : public Stats {
public:
	int SPEED = 100;
	PlayerStats(Entity* owner) : Stats(owner) {	}
};