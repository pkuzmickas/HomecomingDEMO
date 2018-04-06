#include "PlayerMovement.h"

bool PlayerMovement::isMoving() {
	for (bool val : moving) {
		if (val) return true;
	}
	return false;
}

void PlayerMovement::update(float deltaTime) {
	Transform* transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	Movement* movement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
	PlayerStats* stats = (PlayerStats*)owner->findComponent(ComponentType::STATS);
	int numOfDirs = 0;
	for (bool dir : moving) if (dir) numOfDirs++;
	float velocity = stats->SPEED * deltaTime;
	if (numOfDirs == 2) {
		velocity *= SIN45;
	}

	if (moving[DOWN]) {
		transform->globalPosY += velocity;
	}
	if (moving[UP]) {
		transform->globalPosY -= velocity;
	}
	if (moving[LEFT]) {
		transform->globalPosX -= velocity;
	}
	if (moving[RIGHT]) {
		transform->globalPosX += velocity;
	}
}
