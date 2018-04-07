#pragma once
#include <SDL.h>
#include "Globals.h"
#include "MapSystem.h"

class CollisionSystem {
public:
	static bool isColliding(SDL_Rect object1, SDL_Rect object2);
	static Collider::ColliderType isCollidingWithEnv(SDL_Rect object);
private:
	CollisionSystem();
};