#pragma once
#include "Entity.h"
class AI : public Component {
public:
	enum State {
		NORMAL,
		ATTACKING
	};
	AI(Entity* owner) : Component(owner, true) {
		
	}
	State state;
	void walkTo(int destX, int destY, int speed);
	virtual void update(float deltaTime);
	bool isWalking() { return walking; }
private:
	bool walking;
	int destX, destY, walkingSpeed;
};