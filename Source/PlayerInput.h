#pragma once
#include "Entity.h"
#include "PlayerStats.h"
#include "PlayerMovement.h"
#include "PlayerAnimator.h"
#include "Collider.h"
#include <vector>
class PlayerInput : public Input {
private:
	PlayerMovement* playerMovement;
	PlayerStats* playerStats;
	Transform* playerTransform;
	PlayerAnimator* playerAnimator;
	
	struct fPoint {
		float x;
		float y;
	};
	float sign(fPoint p1, fPoint p2, fPoint p3);
	bool pointInTriangle(fPoint pt, fPoint v1, fPoint v2, fPoint v3);
	void animateByMouse();
public:
	PlayerInput(Entity* owner);
	void update(float deltaTime);
};