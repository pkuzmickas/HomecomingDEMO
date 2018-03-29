#include "Director.h"

using namespace std;

Director::Director(SDL_Renderer* renderer) {
	this->renderer = renderer;
	graphics = new Graphics(renderer);
	Map test(renderer, "demoMap.tmx");
	player = new Entity();
	
	Transform* playerTransform = new Transform(player, 48, 48, 100, 100);
	Drawable* playerDrawable = new Drawable(player, IMG_LoadTexture(renderer, ASSET_DIR"player.png"), "Player", Globals::Layers::PLAYER);

	player->addComponent(playerTransform);
	player->addComponent(playerDrawable);
	graphics->addToDraw(player);
}

Director::~Director() {
	delete graphics;
	delete player;
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

