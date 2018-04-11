#include "EncounterScene.h"

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());
	createPlayer(1250, 700);
	CameraSystem::centerAround(0, 700, MapSystem::getWidth(), MapSystem::getHeight());
	PlayerSystem::disableMovement();
	wait(1, "init camera");
}

void EncounterScene::update(float deltaTime) {
	Scene::update(deltaTime);
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
	
}
