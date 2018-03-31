#include "Director.h"

using namespace std;

Director::Director(SDL_Renderer* renderer) {
	this->renderer = renderer;
	graphics = new Graphics(renderer);
	
	curScene = new EncounterScene(renderer, graphics);
	
	
}

Director::~Director() {
	delete graphics;

	delete curScene;
}

void Director::startGame() {
	while (gameRunning) {
		getInput();
		update();
		graphics->render();
	}
}

void Director::getInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			gameRunning = false;
			break;
		}
	}
}

void Director::update() {
	
}

