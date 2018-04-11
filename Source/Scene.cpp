#include "Scene.h"

Scene::~Scene() {
	if(player) delete player;
	MapSystem::deleteMap();
}

void Scene::update(float deltaTime) {
	if (player) player->update(deltaTime);
	if (curAction == "waiting") {
		if (SDL_GetTicks() - waitTimePassed > waitSeconds * 1000) {
			waitSeconds = 0;
			waitTimePassed = 0;
			curAction = nextAction;
			nextAction = "";
		}
	}
}

Scene::Scene(SDL_Renderer * renderer, Graphics * graphics) {
	this->renderer = renderer;
	this->graphics = graphics;

}

void Scene::createPlayer(int globalPosX, int globalPosY, PlayerAnimator::LookDirection lookDirection) {
	player = PlayerSystem::createPlayer(globalPosX, globalPosY, IMG_LoadTexture(renderer, ASSET_DIR"player.png"));
	graphics->addToDraw(player);
}

void Scene::wait(float waitSeconds, std::string nextAction) {
	this->waitSeconds = waitSeconds;
	this->nextAction = nextAction;
	waitTimePassed = SDL_GetTicks();
	curAction = "waiting";
}
