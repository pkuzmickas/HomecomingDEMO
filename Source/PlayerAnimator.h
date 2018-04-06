#pragma once
#include "Animator.h"
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
	void update(float deltaTime) {
		// DEFAULT STANCE
		curSrcRect->y = direction * curSrcRect->h;
		curSrcRect->x = 1 * curSrcRect->w;

		Animator::update(deltaTime);

	}

};