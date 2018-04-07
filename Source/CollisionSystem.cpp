#include "CollisionSystem.h"

bool CollisionSystem::isColliding(SDL_Rect object1, SDL_Rect object2) {
	if (object1.x < object2.x + object2.w &&
		object1.x + object2.w > object2.x &&
		object1.y < object2.y + object2.h &&
		object1.h + object1.y > object2.y) {

		return true;

	}
	return false;
}

Collider::ColliderType CollisionSystem::isCollidingWithEnv(SDL_Rect object)
{
	int col1 = object.x / Globals::TILE_SIZE;
	int col2 = (object.x + object.w) / Globals::TILE_SIZE;
	int row1 = object.y / Globals::TILE_SIZE;
	int row2 = (object.y + object.h) / Globals::TILE_SIZE;

	auto map = MapSystem::getMap();
	for (auto ent : map[row1][col1]) {
		Collider* col = (Collider*) ent->findComponent(ComponentType::COLLIDER);
		if (col) {
			return col->colType;
		}
	}
	for (auto ent : map[row2][col2]) {
		Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
		if (col) {
			return col->colType;
		}
	}
	// write a sick alg to check whether the object is colliding with the environment and if it is with what type of collider :)
	/*if (row1 >= colMap.size() || col1 >= colMap[row1].size() || row2 >= colMap.size() || col2 >= colMap[row2].size()) {
		return 1;
	}
	if (colMap[row1][col1] > 0 || colMap[row1][col2] > 0 || colMap[row2][col1] > 0 || colMap[row2][col2] > 0) {
		return 1;
	}

	/*if (colMap[row1][col1] == 119 || colMap[row1][col2] == 119 || colMap[row2][col1] == 119 || colMap[row2][col2] == 119) {
	return 0;
	}*/

	return Collider::ColliderType::NONE;
}
