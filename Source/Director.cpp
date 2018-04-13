#include "Director.h"
#include "AIComponent.h"
using namespace std;

Director::Director(SDL_Renderer* renderer) {
	this->renderer = renderer;
	graphics = new Graphics(renderer);
	
	curScene = new EncounterScene(renderer, graphics);
	
	// Starts to calculate the running time of the game to create the deltaTime (time passed since last frame)
	runTime = SDL_GetTicks();
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
		Globals::GetFrameEvents().push_back(e);
	}
	for (auto e : Globals::GetFrameEvents()) {
		switch (e.type) {
			case SDL_QUIT:
				gameRunning = false;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_t) {
					graphics->debug = !graphics->debug;
				}
				if (e.key.keysym.sym == SDLK_y) {
					AIComponent* ai = (AIComponent*)PlayerSystem::getPlayer()->findComponent(ComponentType::AI);
					ai->walkTo(100, 700, 300);
				}
				break;
		}
	}

	
}

void Director::update() {
	deltaTime = SDL_GetTicks() - runTime;
	deltaTimeInSeconds = (float)deltaTime / 1000;
	//cout << deltaTimeInSeconds << endl;
	runTime += deltaTime;

	
	curScene->update(deltaTimeInSeconds);
	
	CameraSystem::update(deltaTimeInSeconds);

	Globals::GetFrameEvents().clear();
}

