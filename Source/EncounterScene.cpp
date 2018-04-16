#include "EncounterScene.h"

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, ASSET_DIR LEVEL_DIR "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());
	CameraSystem::setUp(MapSystem::getWidth(), MapSystem::getHeight()); // need to set up the camera right when the map is loaded

	setup();

}

void EncounterScene::setup() {

	createPlayer(1250, 700);

	// Spawning npcs
	oldman = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "oldman.png");
	soldier = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "soldier.png");
	Entity* oldmanEntity = NPCSystem::createNPC(370, 700, 48, 48, Globals::Layers::PLAYER, oldman, "oldman");
	Entity* soldierEntity = NPCSystem::createNPC(300, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier1");
	Entity* soldier2Entity = NPCSystem::createNPC(440, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier2");
	graphics->addToDraw(oldmanEntity);
	graphics->addToDraw(soldierEntity);
	graphics->addToDraw(soldier2Entity);
	entities.push_back(oldmanEntity);
	entities.push_back(soldierEntity);
	entities.push_back(soldier2Entity);


	// Spawning trees
	tree = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "tree.png");
	treeNoLeaves = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "treeNoLeaves.png");
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < MapSystem::getWidth() / 103 + 1; i++) {
			Entity* e = SceneDesignSystem::createTree(i * 103, 550 + j * 150, (Globals::Layers)(Globals::Layers::BACKGROUND2 + j), treeNoLeaves);
			graphics->addToDraw(e);
			entities.push_back(e);
			if (i * 122 < MapSystem::getWidth()) {
				Entity* e2 = SceneDesignSystem::createTree(i * 122, 475 + j * 305, (Globals::Layers)(Globals::Layers::BACKGROUND1 + 3 * j), tree);
				graphics->addToDraw(e2);
				entities.push_back(e2);
			}
		}
	}

	

	CameraSystem::centerAround(0, 700);
	PlayerSystem::disableMovement();
	wait(1, "init camera");

}

void EncounterScene::update(float deltaTime) {
	Scene::update(deltaTime);
	for (auto ent : entities) {
		ent->update(deltaTime);
	}
	if (curAction == "init camera") {
		//CameraSystem::moveCamera(1000, (int)CameraSystem::posY, 200);
		curAction = "camera moving";
	}
	if (curAction == "camera moving") {
		if (!CameraSystem::isCameraMoving()) {
			wait(1, "dialogue start");
		}
	}
	if (curAction == "dialogue start") {
		PlayerSystem::enableMovement();
		Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
		//CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
		curAction = "";
	}
	
}

EncounterScene::~EncounterScene() {
	SDL_DestroyTexture(tree);
	SDL_DestroyTexture(treeNoLeaves);
	SDL_DestroyTexture(oldman);
	SDL_DestroyTexture(soldier);
	for (auto tree : entities) {
		delete tree;
	}
}


