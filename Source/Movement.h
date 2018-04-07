#pragma once
#include "Entity.h"
#include "PlayerStats.h"
#include "CollisionSystem.h"
class Movement : public Component {
public:
	int velX;
	int velY;

	virtual void update(float deltaTime);

	Movement(Entity* owner, int velX = 0, int velY = 0);
};