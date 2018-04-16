#include "EncounterScene.h"

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, ASSET_DIR LEVEL_DIR "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());
	CameraSystem::setUp(MapSystem::getWidth(), MapSystem::getHeight()); // need to set up the camera right when the map is loaded

	setup();

}

void EncounterScene::setup() {

	createPlayer(1700, 700, Animator::LookDirection::LEFT);

	// Spawning npcs
	oldman = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "oldman.png");
	soldier = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "soldier.png");
	Entity* oldmanEntity = NPCSystem::createNPC(370, 700, 48, 48, Globals::Layers::PLAYER, oldman, "oldman");
	Entity* soldierEntity = NPCSystem::createNPC(300, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier1");
	Entity* soldier2Entity = NPCSystem::createNPC(440, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier2");
	oldmanAI = (AIComponent*)oldmanEntity->findComponent(ComponentType::AI);
	soldierAI = (AIComponent*)soldierEntity->findComponent(ComponentType::AI);
	soldier2AI = (AIComponent*)soldier2Entity->findComponent(ComponentType::AI);
	graphics->addToDraw(oldmanEntity);
	graphics->addToDraw(soldierEntity);
	graphics->addToDraw(soldier2Entity);
	entities.push_back(oldmanEntity);
	entities.push_back(soldierEntity);
	entities.push_back(soldier2Entity);
	Transform* oldmanT = (Transform*)oldmanEntity->findComponent(ComponentType::TRANSFORM);
	CameraSystem::follow(&oldmanT->globalPosX, &oldmanT->globalPosY);


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

	

	PlayerSystem::disableMovement();
	curAction = "start walking";

}

void EncounterScene::preFightScenario() {
	if (curAction == "start walking") {
		soldier2AI->walkTo(1150, 700, 80);
		oldmanAI->walkTo(1080, 700, 80);
		soldierAI->walkTo(1010, 700, 80);
		curAction = "npcs walking";
	}
	if (curAction == "npcs walking") {
		if (!soldierAI->isWalking()) {
			wait(2, "camera switch 1");
		}
	}
	if (curAction == "camera switch 1") {
		CameraSystem::detachCamera();
		Transform* soldierTransform = (Transform*)(soldierAI->owner->findComponent(ComponentType::TRANSFORM));
		CameraSystem::moveCamera(soldierTransform->globalPosX - 100, CameraSystem::posY, 100);

		curAction = "camera moving 1";
	}
	if (curAction == "camera moving 1") {
		if (!CameraSystem::isCameraMoving()) {
			// DIALOGUE HAPPENS HERE
			wait(5, "move camera to player");
		}
	}
	if (curAction == "move camera to player") {
		Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
		CameraSystem::moveCamera(playerTransform->globalPosX - Globals::SCREEN_WIDTH / 2, CameraSystem::posY, 400);
		curAction = "camera moving 2";
	}
	if (curAction == "camera moving 2") {
		if (!CameraSystem::isCameraMoving()) {
			Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
			CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
			PlayerSystem::enableMovement();
			curAction = "";
		}
	}
}

void EncounterScene::update(float deltaTime) {
	Scene::update(deltaTime);
	for (auto ent : entities) {
		ent->update(deltaTime);
	}
	
	preFightScenario();
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


