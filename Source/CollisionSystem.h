#pragma once
#include <SDL.h>
class CollisionSystem {
public:
	static bool isColliding(SDL_Rect object1, SDL_Rect object2) {
		if (object1.x < object2.x + object2.w &&
			object1.x + object2.w > object2.x &&
			object1.y < object2.y + object2.h &&
			object1.h + object1.y > object2.y) {

			return true;

		}
		return false;
	}
private:
	CollisionSystem();
};