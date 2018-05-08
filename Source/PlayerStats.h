#pragma once
#include "Component.h"

class PlayerStats : public Stats {
public:
	int SPEED = 250;
	int dashAttackDmg = 40;
	int fSlashAttackDmg = 60;
	PlayerStats(Entity* owner) : Stats(owner) {	}
};