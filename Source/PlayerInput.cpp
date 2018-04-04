#include "PlayerInput.h"
#include <iostream>
PlayerInput::PlayerInput(Entity* owner) : Input(owner) {
	playerMovement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
	playerStats = (PlayerStats*)owner->findComponent(ComponentType::STATS);

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
			}
			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				playerMovement->moving[playerMovement->LEFT] = true;
			}
			if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
				playerMovement->moving[playerMovement->UP] = true;
			}
			if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
				playerMovement->moving[playerMovement->DOWN] = true;
			}

		}
			break;

		case SDL_KEYUP: {
			if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
				playerMovement->moving[playerMovement->RIGHT] = false;
			}
			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				playerMovement->moving[playerMovement->LEFT] = false;
			}
			if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
				playerMovement->moving[playerMovement->UP] = false;
			}
			if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
				playerMovement->moving[playerMovement->DOWN] = false;
			}
			break;
		}
		}

	}
}