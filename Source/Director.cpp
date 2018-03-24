#include "Director.h"

using namespace std;

Director::Director(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

Director::~Director() {

}

void Director::startGame() {
	while (gameRunning) {
		getInput();
		update();
		draw();
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

void Director::draw() {

}