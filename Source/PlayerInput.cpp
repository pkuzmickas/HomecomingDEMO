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
		int numOfDirs = 0;
		switch (e.type) {

			

		case SDL_KEYDOWN: {
			// Handles the movement keys
			if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
				playerMovement->velX = (float)playerStats->SPEED;
			}
			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				playerMovement->velX = (float)-playerStats->SPEED;
			}
			if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
				playerMovement->velY = (float)-playerStats->SPEED;
			}
			if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
				playerMovement->velY = (float)playerStats->SPEED;
			}

		}
			break;

		case SDL_KEYUP: {
			if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
				playerMovement->velX = 0;
			}
			if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
				playerMovement->velX = 0;
			}
			if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
				playerMovement->velY = 0;
			}
			if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
				playerMovement->velY = 0;
			}
			break;
		}
		}

	}
}