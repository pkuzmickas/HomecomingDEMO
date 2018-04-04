#include "Entity.h"

void Entity::addComponent(Component * comp) {
	if (hasComponent(comp->type)) {
		std::cout << "Component already exists! Overriding the old one. TYPE: " <<comp->type<< std::endl;

		delete components[comp->type];
	}
	components[comp->type] = comp;	
	mask = mask | comp->type;
}

Component * Entity::findComponent(ComponentType comp) {

	if (!hasComponent(comp)) {
		std::cout << "Component could not be found! TYPE:" <<comp<< std::endl;
		return NULL;
	}
	else {
		return components[comp];
	}

}

bool Entity::hasComponent(ComponentType comp)
{
	if (comp != (mask & comp)) {
		return false;
	}
	return true;
}

void Entity::update(float deltaTime) {
	for (auto comp : components) {
		if (comp.second->updatable) {
			comp.second->update(deltaTime);
		}
	}

	// General updates

	// If there is a movement component, update the movement each frame
	// This is in Entity since it is general and would apply to all movements
	if (ComponentType::MOVEMENT == (mask & ComponentType::MOVEMENT)) {
		Transform* transform = (Transform*)findComponent(ComponentType::TRANSFORM);
		Movement* movement = (Movement*)findComponent(ComponentType::MOVEMENT);
		
		int numOfDirs = 0;
		for (bool dir : movement->moving) if (dir) numOfDirs++;
		float velocity = movement->speed * deltaTime;
		if (numOfDirs == 2) {
			velocity *= SIN45;
		}
		
		if (movement->moving[movement->DOWN]) {
			transform->globalPosY += velocity;
		}
		if (movement->moving[movement->UP]) {
			transform->globalPosY -= velocity;
		}
		if (movement->moving[movement->LEFT]) {
			transform->globalPosX -= velocity;
		}
		if (movement->moving[movement->RIGHT]) {
			transform->globalPosX += velocity;
		}


	}
	
}

Entity::~Entity() {
	for (auto it : components) {
		delete it.second;
	}
}

