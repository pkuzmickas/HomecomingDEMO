#include "PlayerInput.h"
#include <iostream>
PlayerInput::PlayerInput(Entity* owner) : Input(owner) {
	playerMovement = (PlayerMovement*)owner->findComponent(ComponentType::MOVEMENT);
	playerStats = (PlayerStats*)owner->findComponent(ComponentType::STATS);
	playerTransform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	playerAnimator = (PlayerAnimator*)owner->findComponent(ComponentType::ANIMATOR);

}

void PlayerInput::update(float deltaTime) {
	std::vector<SDL_Event> events = Globals::GetFrameEvents();
	//std::cout << "EVENT SIZE IN PLAYER INPUT:" << events.size() << std::endl;
	for (auto e : events) {
		switch (e.type) {

			

		case SDL_KEYDOWN: {
			// Handles the movement keys
			if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
				playerMovement->moving[playerMovement->RIGHT] = true;
				playerAnimator->animateMovement();
			}
			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				playerMovement->moving[playerMovement->LEFT] = true;
				playerAnimator->animateMovement();
			}
			if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
				playerMovement->moving[playerMovement->UP] = true;
				playerAnimator->animateMovement();
			}
			if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
				playerMovement->moving[playerMovement->DOWN] = true;
				playerAnimator->animateMovement();
			}

		}
			break;

		case SDL_KEYUP: {
			if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
				playerMovement->moving[playerMovement->RIGHT] = false;
				if (!playerMovement->isMoving()) {
					playerAnimator->stopAnimation();
				}
			}
			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				playerMovement->moving[playerMovement->LEFT] = false;
				if (!playerMovement->isMoving()) {
					playerAnimator->stopAnimation();
				}
			}
			if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
				playerMovement->moving[playerMovement->UP] = false;
				if (!playerMovement->isMoving()) {
					playerAnimator->stopAnimation();
				}
			}
			if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
				playerMovement->moving[playerMovement->DOWN] = false;
				if (!playerMovement->isMoving()) {
					playerAnimator->stopAnimation();
				}
			}
			break;
		}
		}

	}

	animateByMouse();
}

/*
Algorithms to detect in which triangle (of the 4) in the screen the mouse is
The following code is taken from:https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
*/
float PlayerInput::sign(fPoint p1, fPoint p2, fPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PlayerInput::pointInTriangle(fPoint pt, fPoint v1, fPoint v2, fPoint v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}
/* Further code is original */

/* Using the algorithms above, detects where the mouse is and sets the player's direction*/
void PlayerInput::animateByMouse() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	fPoint mouse;
	mouse.x = (float)mouseX;
	mouse.y = (float)mouseY;
	fPoint middle;
	middle.x = playerTransform->globalPosX - CameraSystem::posX + playerTransform->width/2;
	middle.y = playerTransform->globalPosY - CameraSystem::posY + playerTransform->height / 2;
	fPoint beginning;
	beginning.x = middle.x - Globals::SCREEN_WIDTH / 2;
	beginning.y = middle.y - Globals::SCREEN_HEIGHT / 2;
	fPoint end;
	end.x = middle.x + Globals::SCREEN_WIDTH / 2;
	end.y = middle.y - Globals::SCREEN_HEIGHT / 2;
	if (pointInTriangle(mouse, middle, beginning, end)) {
		playerAnimator->direction = PlayerAnimator::LookDirection::UP;
		if(playerMovement->isMoving())
		playerAnimator->animateMovement();
	}
	beginning.x = middle.x + Globals::SCREEN_WIDTH / 2;
	beginning.y = middle.y - Globals::SCREEN_HEIGHT / 2;
	end.x = middle.x + Globals::SCREEN_WIDTH / 2;
	end.y = middle.y + Globals::SCREEN_HEIGHT / 2;
	if (pointInTriangle(mouse, middle, beginning, end)) {
		playerAnimator->direction = PlayerAnimator::LookDirection::RIGHT;
		if (playerMovement->isMoving())
		playerAnimator->animateMovement();
	}
	beginning.x = middle.x - Globals::SCREEN_WIDTH / 2;
	beginning.y = middle.y + Globals::SCREEN_HEIGHT / 2;
	end.x = middle.x + Globals::SCREEN_WIDTH / 2;
	end.y = middle.y + Globals::SCREEN_HEIGHT / 2;
	if (pointInTriangle(mouse, middle, beginning, end)) {
		playerAnimator->direction = PlayerAnimator::LookDirection::DOWN;
		if (playerMovement->isMoving())
		playerAnimator->animateMovement();
	}
	beginning.x = middle.x - Globals::SCREEN_WIDTH / 2;
	beginning.y = middle.y + Globals::SCREEN_HEIGHT / 2;
	end.x = middle.x - Globals::SCREEN_WIDTH / 2;
	end.y = middle.y - Globals::SCREEN_HEIGHT / 2;
	if (pointInTriangle(mouse, middle, beginning, end)) {
		playerAnimator->direction = PlayerAnimator::LookDirection::LEFT;
		if (playerMovement->isMoving())
		playerAnimator->animateMovement();
	}
}
