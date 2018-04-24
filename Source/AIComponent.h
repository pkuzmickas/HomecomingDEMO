#pragma once
#include "Entity.h"
#include "Movement.h"
#include "Animator.h"
#include "PathfindingSystem.h"
class AIComponent : public Component {
public:
	enum State {
		NORMAL,
		ATTACKING
	};
	AIComponent(Entity* owner);
	State state = NORMAL;
	void walkTo(int destX, int destY, int speed);
	virtual void update(float deltaTime);
	bool isWalking() { return walking; }
	void knockBack(int dist, int speed);
private:
	bool walking, isKnocked;
	int destX, destY, walkingSpeed;
	Movement* movement;
	Animator* animator;
	Transform* transform;
	std::vector<SDL_Point> path;
	int curPathIndex;
	int knockTimePassed;
	int knockSpeed;
	int resultX;
};