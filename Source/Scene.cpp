#include "Scene.h"

Scene::~Scene() {
	if(player) delete player;
	MapSystem::deleteMap();
}

void Scene::update(float deltaTime) {
	if (player) player->update(deltaTime);
}

Scene::Scene(SDL_Renderer * renderer, Graphics * graphics) {
	this->renderer = renderer;
	this->graphics = graphics;

}

void Scene::createPlayer(int globalPosX, int globalPosY) {
	player = PlayerSystem::createPlayer(globalPosX, globalPosY, IMG_LoadTexture(renderer, ASSET_DIR"player.png"));
	CameraSystem::centerAround(globalPosX, globalPosY, MapSystem::getWidth(), MapSystem::getHeight());
	graphics->addToDraw(player);
}
