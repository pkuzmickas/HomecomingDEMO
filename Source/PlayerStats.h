#pragma once
#include "Component.h"

class PlayerStats : public Stats {
public:
	int SPEED = 150;
	PlayerStats(Entity* owner) : Stats(owner) {	}
};