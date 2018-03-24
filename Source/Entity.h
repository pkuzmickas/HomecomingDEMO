#pragma once
#include "Component.h"
#include <unordered_map>
class Entity {
public:
	Transform* transform;
	int mask;
	void addComponent(Component* comp);
	virtual void update(float deltaTime);
	Component* findComponent(ComponentType comp);

private:
	std::unordered_map<ComponentType, Component*> components;
};