#pragma once
#include "Scene.h"
class EncounterScene : public Scene {
public:
	explicit EncounterScene(SDL_Renderer* renderer, Graphics* graphics);
	~EncounterScene();
};