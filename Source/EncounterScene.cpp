#include "EncounterScene.h"
#pragma once

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());
	createPlayer(100, 700);
}

EncounterScene::~EncounterScene() {
	
}
