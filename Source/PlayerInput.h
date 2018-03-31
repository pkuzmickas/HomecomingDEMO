#pragma once
#include "Component.h"
class PlayerInput : Input {
public:
	void update();
	PlayerInput(Entity* owner);
};