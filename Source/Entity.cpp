#include "Entity.h"

void Entity::addComponent(Component * comp) {
	if (components.find(comp->type) != components.end()) {
		std::cout << "Component already exists! Overriding the old one." << std::endl;
		delete components[comp->type];
	}
	components[comp->type] = comp;	
	mask = mask | comp->type;
}

Component * Entity::findComponent(ComponentType comp) {

	if (comp != (mask & comp)) {
		std::cout << "Component could not be found!" << std::endl;
		return NULL;
	}
	else {
		return components[comp];
	}

}

Entity::~Entity() {
	for (auto it : components) {
		delete it.second;
	}
}
