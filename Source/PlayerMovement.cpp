#include "PlayerMovement.h"

bool PlayerMovement::isMoving() {
	for (bool val : moving) {
		if (val) return true;
	}
	return false;
}

void PlayerMovement::update(float deltaTime) {
	
	int numOfDirs = 0;
	for (bool dir : moving) if (dir) numOfDirs++;
	float velocity = stats->SPEED * deltaTime;
	if (numOfDirs == 2) {
		velocity *= SIN45;
	}
	std::cout << CollisionSystem::isCollidingWithEnv(collider->colBox) << std::endl;
	// ABOVE DOES NOT WORK SINCE U NEED TO UPDATE ENTITIES COLBOX ON MOVEMENT OR CAMERA MOVEMENT, CREATE AN UPDATE FOR THE COLLIDER COMPONENT?
	if (moving[DOWN]) {
		float estimateLoc = transform->globalPosY + velocity;

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
