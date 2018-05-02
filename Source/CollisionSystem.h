#pragma once
#include <SDL.h>
#include "Globals.h"
#include "MapSystem.h"
#include "Collider.h"
class CollisionSystem {
public:
	static bool isColliding(SDL_Rect object1, SDL_Rect object2);
	static Collider* isCollidingWithEnv(Collider* object);
	static Collider* isCollidingWithEnv(SDL_Rect object);
	static Collider* isCollidingWithObjects(Collider* object, vector<string> exceptions);
	static Collider* isCollidingWithObjects(SDL_Rect object, vector<string> exceptions);
	static std::vector<Collider*> collidersInScene;
	static bool removeCollider(Collider* col);
private:
	CollisionSystem();
};