#include "Scene.h"

Scene::~Scene() {
	if(player) delete player;
	MapSystem::deleteMap();
	DialogueSystem::cleanup();
	UIDesignSystem::cleanup();
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
	DialogueSystem::update(deltaTime);
	
}

Scene::Scene(SDL_Renderer * renderer, Graphics * graphics) {
	this->renderer = renderer;
	this->graphics = graphics;
	DialogueSystem::setup(graphics, renderer);
	UIDesignSystem::setup(renderer, graphics);
	
}

void Scene::createPlayer(int globalPosX, int globalPosY, PlayerAnimator::LookDirection lookDirection) {
	player = PlayerSystem::createPlayer(globalPosX, globalPosY, IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "player.png"), renderer, graphics,lookDirection);
	graphics->addToDraw(player);
}

void Scene::wait(float waitSeconds, std::string nextAction) {
	this->waitSeconds = waitSeconds;
	this->nextAction = nextAction;
	waitTimePassed = (int)SDL_GetTicks();
	curAction = "waiting";
}
