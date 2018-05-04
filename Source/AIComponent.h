#pragma once
#include "Entity.h"
#include "Movement.h"
#include "Animator.h"
#include "PathfindingSystem.h"
#include "UIDesignSystem.h"
class AIComponent : public Component {
public:
	enum State {
		NORMAL,
		ATTACKING,
		DEAD
	};
	AIComponent(Entity* owner);
	State state = NORMAL;
	void walkTo(int destX, int destY, int speed);
	virtual void update(float deltaTime);
	bool isWalking() { return walking; }
	bool isKnocked() { return knocked; }
	void knockBack(int dist, int speed, Animator::LookDirection dir, std::string attackName);
	void attack(Entity* target);
protected:
	void stopWalking();
	void calculatePath(int destX, int destY);

	Movement * movement;
	Animator* animator;
	Transform* transform;
	Collider* collider;
	Stats* stats;
	Entity* target;
	Transform* targetTransform;
	bool walking, knocked;
	int destX, destY, walkingSpeed, walkingDir;
	
	std::vector<SDL_Point> path;
	int curPathIndex;
	int result; // for some computation (temp global int)
	Animator::LookDirection knockDir;
	std::string knockedByAttackName = "";

};