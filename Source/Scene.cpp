#include "Scene.h"

Scene::~Scene() {
	delete player;
	delete map;
}

void Scene::update(float deltaTime) {

}

Scene::Scene(SDL_Renderer * renderer, Graphics * graphics) {
	this->renderer = renderer;
	this->graphics = graphics;

}

void Scene::createPlayer(int globalPosX, int globalPosY) {
	if (!map) {
		std::cout << "Could not create player! Map is not initialized!" << std::endl;
		return;
	}
	player = new Entity();
	Camera::centerAround(globalPosX, globalPosY, map->getWidth(), map->getHeight());
	Transform* playerTransform = new Transform(player, 48, 48, globalPosX, globalPosY);
	Drawable* playerDrawable = new Drawable(player, IMG_LoadTexture(renderer, ASSET_DIR"player.png"), "player", Globals::Layers::PLAYER);
	
	player->addComponent(playerTransform);
	player->addComponent(playerDrawable);
	graphics->addToDraw(player);
}
