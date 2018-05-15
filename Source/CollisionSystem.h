#pragma once
#include <SDL.h>
#include "Globals.h"
#include "MapSystem.h"
#include "Collider.h"
class CollisionSystem {
public:
	// Checks whether two rectangles are colliding
	static bool isColliding(SDL_Rect object1, SDL_Rect object2);
	// Checks whether a Collider object is colliding with the environment
	static Collider* isCollidingWithEnv(Collider* object);
	// Checks whether a certain rectangle is colliding with the environment
	static Collider* isCollidingWithEnv(SDL_Rect object);
	// Checks whether a Collider object is colliding with another object in the scene
	static Collider* isCollidingWithObjects(Collider* object, vector<string> exceptions);
	// Checks whether a certain rectangle is colliding with another object in the scene
	static Collider* isCollidingWithObjects(SDL_Rect object, vector<string> exceptions);
	// A vector with all the colliders in scene
	static std::vector<Collider*> collidersInScene;
	// A function to remove a collider from the scene
	static bool removeCollider(Collider* col);
private:
	CollisionSystem();
};