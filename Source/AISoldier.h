#pragma once
#include "AIComponent.h"

class AISoldier : public AIComponent {
public:
	AISoldier(Entity* owner);
	void update(float deltaTime);
private:
	enum subStates {
		NONE,
		FINDING,
		SLASHING
	};
	subStates subState = NONE;
	
};
