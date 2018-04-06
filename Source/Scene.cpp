#include "Scene.h"

Scene::~Scene() {
	if(player) delete player;
	if(map) delete map;
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
	Camera::centerAround(globalPosX, globalPosY, map->getWidth(), map->getHeight());
	graphics->addToDraw(player);
}
