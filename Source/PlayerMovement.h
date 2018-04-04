#pragma once
#include "Movement.h"

class PlayerMovement : public Movement {
public:
	enum Directions {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	std::vector<bool> moving = { false, false, false, false };
	
	void update(float deltaTime);

	PlayerMovement(Entity* owner, std::vector<bool>* moving = NULL) : Movement(owner) {

		if (moving) this->moving = *moving;
		type = MOVEMENT;

	}
};