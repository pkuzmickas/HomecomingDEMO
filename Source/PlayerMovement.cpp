#include "PlayerMovement.h"

bool PlayerMovement::isMoving() {
	for (bool val : moving) {
		if (val) return true;
	}
	return false;
}


void PlayerMovement::update(float deltaTime) {
	Movement::update(deltaTime);
	if (!movementEnabled) return;
	int numOfDirs = 0;
	for (bool dir : moving) if (dir) numOfDirs++;
	float velF = stats->SPEED * deltaTime;
	if (numOfDirs == 2) {
		velF *= SIN45;
	}
	if (numOfDirs != 0) {

		if (moving[DOWN]) {
			int prev = collider->colBox.y;
			collider->colBox.y += (int)velF + 1; // Adding one to remove getting stuck at the walls
			if (CollisionSystem::isCollidingWithEnv(collider) == NULL && CollisionSystem::isCollidingWithObjects(collider, { "slashAttack" , "fslash" }) == NULL) {
				transform->globalPosY += velF;
				collider->colBox.y -= 1;
			}
			else {
				collider->colBox.y = prev;
			}
		}
		if (moving[UP]) {
			int prev = collider->colBox.y;
			collider->colBox.y -= ((int)velF + 1);
			if (CollisionSystem::isCollidingWithEnv(collider) == NULL && CollisionSystem::isCollidingWithObjects(collider, { "slashAttack" , "fslash" }) == NULL) {
				transform->globalPosY -= velF;
				collider->colBox.y += 1;

			}
			else {
				collider->colBox.y = prev;
			}
		}
		if (moving[LEFT]) {
			int prev = collider->colBox.x;
			collider->colBox.x -= ((int)velF + 1);
			if (CollisionSystem::isCollidingWithEnv(collider) == NULL && CollisionSystem::isCollidingWithObjects(collider, { "slashAttack", "fslash" }) == NULL) {
				transform->globalPosX -= velF;
				collider->colBox.x += 1;
			}
			else {
				collider->colBox.x = prev;
			}
		}
		if (moving[RIGHT]) {
			int prev = collider->colBox.x;
			collider->colBox.x += (int)velF + 1;
			if (CollisionSystem::isCollidingWithEnv(collider) == NULL && CollisionSystem::isCollidingWithObjects(collider, { "slashAttack" , "fslash" }) == NULL) {
				transform->globalPosX += velF;
				collider->colBox.x -= 1;
			}
			else {
				collider->colBox.x = prev;
			}
		}
	}

}
