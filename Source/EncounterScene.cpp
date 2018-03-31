#include "EncounterScene.h"
#pragma once

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	map = new Map(renderer, "demoMap.tmx");
	graphics->addMap(map->getMap());
	createPlayer(100, 700);
}

EncounterScene::~EncounterScene() {
	
}
