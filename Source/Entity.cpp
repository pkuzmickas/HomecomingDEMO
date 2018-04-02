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
		//std::cout << "DOING IT "<< ((Drawable*)components[ComponentType::DRAWABLE])->ID<<" SPEED:"<< movement->velX<< std::endl;
		if (movement->velX != 0 && movement->velY != 0) {
			movement->velX *= SIN45;
			movement->velY *= SIN45;
			
		}
		//std::cout << "WAS POSITION: " << transform->globalPosX;
		transform->globalPosX += movement->velX * deltaTime; // 0.1f - speed modifier
		transform->globalPosY += movement->velY * deltaTime;
		//std::cout << " NEW POS:" << transform->globalPosX << " VEL WAS:"<< movement->velX <<" DT WAS:"<<deltaTime<< std::endl;

	}
	
}

Entity::~Entity() {
	for (auto it : components) {
		delete it.second;
	}
}

