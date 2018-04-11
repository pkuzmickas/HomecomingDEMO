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
	float velF = stats->SPEED * deltaTime;
	if (numOfDirs == 2) {
		velF *= SIN45;
	}
	int velocity = (int)velF;
	if (numOfDirs != 0) {
		
		if (moving[DOWN]) {
			collider->colBox.y += velocity;
			if (CollisionSystem::isCollidingWithEnv(collider->colBox) == Collider::ColliderType::NONE) {
				transform->globalPosY += velocity;
				
			}
		}
		if (moving[UP]) {
			collider->colBox.y -= velocity;
			if (CollisionSystem::isCollidingWithEnv(collider->colBox) == Collider::ColliderType::NONE) {
				transform->globalPosY -= velocity;
			}
		}
		if (moving[LEFT]) {
			collider->colBox.x -= velocity;
			if (CollisionSystem::isCollidingWithEnv(collider->colBox) == Collider::ColliderType::NONE) {
				transform->globalPosX -= velocity;
			}
		}
		if (moving[RIGHT]) {
			collider->colBox.x += velocity;
			if (CollisionSystem::isCollidingWithEnv(collider->colBox) == Collider::ColliderType::NONE) {
				transform->globalPosX += velocity;
			}
		}
		//CameraSystem::centerAround((int)transform->globalPosX, (int)transform->globalPosY, MapSystem::getWidth(), MapSystem::getHeight());
	}

}
