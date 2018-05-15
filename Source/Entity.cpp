#include "Entity.h"

void Entity::addComponent(Component * comp) {
	// Checks whether the component type already exists and if so deletes the previous one
	if (hasComponent(comp->type)) {
		std::cout << "Component already exists! Overriding the old one. TYPE: " <<comp->type<< std::endl;

		delete components[comp->type];
	}
	// Adds the new component to the map
	components[comp->type] = comp;	
	// Adds it to the mask which helps for faster finding whether a component exists using binary operations
	mask = mask | comp->type;
}

Component * Entity::findComponent(ComponentType comp) {
	// Checks whether the Entity has the component and returns it
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
	// Checks whether the entity has the component using the mask and a binary operation
	if (comp != (mask & comp)) {
		return false;
	}
	return true;
}

void Entity::update(float deltaTime) {
	// Goes through all of the components and checks whether they are updatable and if so, updates them
	for (auto comp : components) {
		if (comp.second->updatable && comp.second->enabled) {
			comp.second->update(deltaTime);
		}
	}
}

Entity::~Entity() {
	// Deletes all of the components when the entity is deleted
	for (auto it : components) {
		delete it.second;
	}
}

