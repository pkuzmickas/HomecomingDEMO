#include "Movement.h"

void Movement::update(float deltaTime) {
	Transform* transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	Movement* movement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
	//Collider* col = (Collider*)owner->findComponent(ComponentType::COLLIDER);
	//if (col) {
//
	//}

	transform->globalPosX += velX * deltaTime;
	transform->globalPosY += velY * deltaTime;
}

Movement::Movement(Entity * owner, int velX, int velY) : Component(owner, true) {
	this->velX = velX;
	this->velY = velY;

	type = MOVEMENT;
}
