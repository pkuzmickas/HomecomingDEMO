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

// The function which starts the game
void Director::startGame() {
	// Shows the start screen
	curScene->showStartScreen();
	// The main game loop
	while (gameRunning) {
		getInput(); // Gets all of the input
		update(); // Updates everything that is updatable
		graphics->render(); // Renders everything that is inside a Drawing Queue onto the screen
	}
}

void Director::getInput() {
	SDL_Event e;
	// Gets all of the events and pushes them onto a vector so that those events can be accessed from anywhere
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
					
					gameRunning = false;
					
				}
				if (e.key.keysym.sym == SDLK_RETURN) {
					if (curScene->isGameOver()) {
						curScene->loadAction("restart");
					}
					if (!curScene->isGameStarted() && !curScene->isControlsWindowOn()) {
						curScene->startScene();
						//curScene->loadAction("restart");
					}
					if (curScene->isControlsWindowOn()) {
						curScene->hideControlsScreen();
					}
				}
				break;
		}
	}

	
}

void Director::update() {
	// Calculates the time passed since last frame
	deltaTime = SDL_GetTicks() - runTime;
	deltaTimeInSeconds = (float)deltaTime / 1000;
	//cout << deltaTimeInSeconds << endl;
	runTime += deltaTime;

	// Updates all of the needed systems and scenes
	curScene->update(deltaTimeInSeconds);
	CameraSystem::update(deltaTimeInSeconds);
	UIDesignSystem::update(deltaTime);

	// Resets the input vector
	Globals::GetFrameEvents().clear();
}

