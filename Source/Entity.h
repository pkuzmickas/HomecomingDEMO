#pragma once
#include "Component.h"
#include <unordered_map>
#include <iostream>
class Entity {
public:
	int mask;
	void addComponent(Component* comp);
	Component* findComponent(ComponentType comp);
	bool hasComponent(ComponentType comp);
	void update(float deltaTime);
	~Entity(); // Deletes all components 
private:
	std::unordered_map<ComponentType, Component*> components;
	
	
};