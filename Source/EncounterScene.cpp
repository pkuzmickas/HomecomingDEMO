#include "EncounterScene.h"

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, ASSET_DIR LEVEL_DIR "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());

	createPlayer(1250, 700);

	tree = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "tree.png");
	treeNoLeaves = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "treeNoLeaves.png");

	Entity* e = SceneDesignSystem::createTree(1400, 700, Globals::Layers::FOREGROUND, treeNoLeaves);
	graphics->addToDraw(e);
	trees.push_back(e);

	CameraSystem::centerAround(0, 700, MapSystem::getWidth(), MapSystem::getHeight());
	PlayerSystem::disableMovement();
	wait(1, "init camera");
}

void EncounterScene::update(float deltaTime) {
	Scene::update(deltaTime);
	for (auto tree : trees) {
		tree->update(deltaTime);
	}
	if (curAction == "init camera") {
		CameraSystem::moveCamera(1000, (int)CameraSystem::posY, 200);
		curAction = "camera moving";
	}
	if (curAction == "camera moving") {
		if (!CameraSystem::isCameraMoving()) {
			wait(1, "dialogue start");
		}
	}
	if (curAction == "dialogue start") {
		PlayerSystem::enableMovement();
	}
	
}

EncounterScene::~EncounterScene() {
	SDL_DestroyTexture(tree);
	SDL_DestroyTexture(treeNoLeaves);
	for (auto tree : trees) {
		delete tree;
	}
}
