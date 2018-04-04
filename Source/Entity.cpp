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
}

Entity::~Entity() {
	for (auto it : components) {
		delete it.second;
	}
}

