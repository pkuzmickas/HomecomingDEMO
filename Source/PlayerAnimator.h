#pragma once
#include "Animator.h"
#include <string>
class PlayerAnimator : public Animator {
public:
	enum LookDirection {
		DOWN,
		LEFT,
		RIGHT,
		UP
	};
	LookDirection direction;
	PlayerAnimator(Entity* owner) : Animator(owner) {
		direction = LookDirection::DOWN;
	}
	void animateMovement() {
		std::string walkingDir = "walking" + std::to_string(direction);
		if(curAnimName()!= walkingDir)
		playAnimation(walkingDir,true);
	}
	void update(float deltaTime) {
		// DEFAULT STANCE
		if (!isAnimating()) {
			curSrcRect->x = 1 * curSrcRect->w;
			curSrcRect->y = direction * curSrcRect->h;
		}
		Animator::update(deltaTime);

	}

};