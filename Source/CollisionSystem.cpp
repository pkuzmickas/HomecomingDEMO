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
	int col1 = (int)((object.x + CameraSystem::posX) / Globals::TILE_SIZE);
	int col2 = (int)((object.x + object.w + CameraSystem::posX) / Globals::TILE_SIZE);
	int row1 = (int)((object.y + CameraSystem::posY) / Globals::TILE_SIZE);
	int row2 = (int)((object.y + object.h + CameraSystem::posY) / Globals::TILE_SIZE);

	auto map = MapSystem::getMap();
	for (Entity* ent : map->at(row1).at(col1)) {
		if (ent->hasComponent(ComponentType::COLLIDER)) {
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (col) {
				return col->colType;
			}
		}
	}
	for (auto ent : map->at(row2).at(col2)) {
		if (ent->hasComponent(ComponentType::COLLIDER)) {
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (col) {
				return col->colType;
			}
		}
	}

	return Collider::ColliderType::NONE;
}
