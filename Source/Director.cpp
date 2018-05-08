#include "Director.h"
#include "AIComponent.h"
using namespace std;

Director::Director(SDL_Renderer* renderer) {
	this->renderer = renderer;
	graphics = new Graphics(renderer);
	
	curScene = new EncounterScene(renderer, graphics);
	//curScene->loadAction("restart");
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
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					if (curScene->isGameOver()) {
						gameRunning = false;
					}
				}
				if (e.key.keysym.sym == SDLK_RETURN) {
					if (curScene->isGameOver()) {
						curScene->loadAction("restart");
					}
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
	UIDesignSystem::update(deltaTime);

	Globals::GetFrameEvents().clear();
}

