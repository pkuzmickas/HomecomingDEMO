#pragma once
#include "Component.h"
#include <unordered_map>
#include <iostream>
class Entity {
public:
	int mask; // A mask that provides information on what components the entity currently has
	bool active = true; // Whether the entity is active in game
	void addComponent(Component* comp); // Adds a component to the entity
	Component* findComponent(ComponentType comp); // Finds and returns a specific component
	bool hasComponent(ComponentType comp); // Checks whether the entity has a specific component
	void update(float deltaTime); // The update function which updates all of the components that need updating
	~Entity(); // Deletes all components 
private:
	// The map of all components that the entity will have
	std::unordered_map<ComponentType, Component*> components;

};