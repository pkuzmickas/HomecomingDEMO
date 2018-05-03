#pragma once
#include "Component.h"

class PlayerStats : public Stats {
public:
	int SPEED = 250;
	int dashAttackDmg = 30;
	int fSlashAttackDmg = 50;
	PlayerStats(Entity* owner) : Stats(owner) {	}
};