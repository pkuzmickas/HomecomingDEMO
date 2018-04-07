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
		transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
		movement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
		stats = (PlayerStats*)owner->findComponent(ComponentType::STATS);
		collider = (Collider*)owner->findComponent(ComponentType::COLLIDER);
		if (moving) this->moving = *moving;
		type = MOVEMENT;

	}
private:
	Transform* transform;
	Movement* movement;
	PlayerStats* stats;
	Collider* collider;
};