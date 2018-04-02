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
	if (!map) {
		std::cout << "Could not create player! Map is not initialized!" << std::endl;
		return;
	}
	player = new Entity();
	Camera::centerAround(globalPosX, globalPosY, map->getWidth(), map->getHeight());
	Transform* playerTransform = new Transform(player, 48, 48, globalPosX, globalPosY);
	player->addComponent(playerTransform);
	PlayerStats* playerStats = new PlayerStats(player);
	player->addComponent(playerStats);
	Drawable* playerDrawable = new Drawable(player, IMG_LoadTexture(renderer, ASSET_DIR"player.png"), "player", Globals::Layers::PLAYER);
	player->addComponent(playerDrawable);
	Movement* playerMovement = new Movement(player);
	player->addComponent(playerMovement);
	PlayerInput* playerInput = new PlayerInput(player);
	player->addComponent(playerInput);
	

	graphics->addToDraw(player);
}
