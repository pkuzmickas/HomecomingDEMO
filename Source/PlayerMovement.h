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
	
	bool isMoving();

	void update(float deltaTime);

	PlayerMovement(Entity* owner, std::vector<bool>* moving = NULL) : Movement(owner) {
		Transform* transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
		Movement* movement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
		PlayerStats* stats = (PlayerStats*)owner->findComponent(ComponentType::STATS);
		if (moving) this->moving = *moving;
		type = MOVEMENT;

	}
private:
	Transform* transform;
	Movement* movement;
	PlayerStats* stats;
};