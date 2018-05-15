#include "CollisionSystem.h"

std::vector<Collider*> CollisionSystem::collidersInScene;

bool CollisionSystem::isColliding(SDL_Rect object1, SDL_Rect object2) {
	if (object1.x < object2.x + object2.w &&
		object1.x + object1.w > object2.x &&
		object1.y < object2.y + object2.h &&
		object1.h + object1.y > object2.y) {

		return true;

	}
	return false;
}

Collider* CollisionSystem::isCollidingWithEnv(Collider* object)
{
	int col1 = (int)((object->colBox.x + CameraSystem::posX) / Globals::TILE_SIZE);
	int col2 = (int)((object->colBox.x + object->colBox.w + CameraSystem::posX) / Globals::TILE_SIZE);
	int row1 = (int)((object->colBox.y + CameraSystem::posY) / Globals::TILE_SIZE);
	int row2 = (int)((object->colBox.y + object->colBox.h + CameraSystem::posY) / Globals::TILE_SIZE);
	// Gets the map matrix and finds the colliders in the environment
	auto map = MapSystem::getMap();
	// Checks the object against those colliders
	for (Entity* ent : map->at(row1).at(col1)) {
		if (ent->hasComponent(ComponentType::COLLIDER)) {
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (col) {
				return col;
			}
		}
	}
	for (auto ent : map->at(row2).at(col2)) {
		if (ent->hasComponent(ComponentType::COLLIDER)) {
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (col) {
				return col;
			}
		}
	}

	return NULL;
}
// Does the same as the previous one just without the Collider object but using a rectangle
Collider * CollisionSystem::isCollidingWithEnv(SDL_Rect object) {

	int col1 = (int)((object.x + CameraSystem::posX) / Globals::TILE_SIZE);
	int col2 = (int)((object.x + object.w + CameraSystem::posX) / Globals::TILE_SIZE);
	int row1 = (int)((object.y + CameraSystem::posY) / Globals::TILE_SIZE);
	int row2 = (int)((object.y + object.h + CameraSystem::posY) / Globals::TILE_SIZE);

	auto map = MapSystem::getMap();
	for (Entity* ent : map->at(row1).at(col1)) {
		if (ent->hasComponent(ComponentType::COLLIDER)) {
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (col) {
				return col;
			}
		}
	}
	for (auto ent : map->at(row2).at(col2)) {
		if (ent->hasComponent(ComponentType::COLLIDER)) {
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (col) {
				return col;
			}
		}
	}

	return NULL;
}

Collider* CollisionSystem::isCollidingWithObjects(Collider* object, vector<string> exceptions) {
	for (auto object2 : collidersInScene) {
		
		// Gets the name of the object in the scene and checks whether its player, if its not then updates it so it is prepared for collision checking (with player its buggy)
		Drawable* d1 = (Drawable*)(object->owner->findComponent(ComponentType::DRAWABLE));
		Drawable* d2 = (Drawable*)(object2->owner->findComponent(ComponentType::DRAWABLE));
		if (d2->ID != "player") {
			object2->update(0);
		}
		bool isExcepted = false;
		// If the object is an exception than it is not colliding
		for (auto name : exceptions) {
			if (name == d2->ID) {
				isExcepted = true;
			}
		}
		if (isExcepted) continue;
		// If there is a collision then returns the collided object
		if (object2 != object && isColliding(object->colBox, object2->colBox)) {

			//cout << d1->ID << " is colliding with " << d2->ID << endl;
			return object2;
		}
	}
	return NULL;
}
// Does the same as the previous one just without the Collider object but using a rectangle
Collider* CollisionSystem::isCollidingWithObjects(SDL_Rect object, vector<string> exceptions) {
	for (auto object2 : collidersInScene) {

		
		Drawable* d2 = (Drawable*)(object2->owner->findComponent(ComponentType::DRAWABLE));
		if (d2->ID != "player") {
			object2->update(0);
		}
		bool isExcepted = false;
		for (auto name : exceptions) {
			if (name == d2->ID) {
				isExcepted = true;
			}
		}
		if (isExcepted) continue;
		if (isColliding(object, object2->colBox)) {

			//cout << "SDL_RECT is colliding with " << d2->ID << endl;
			return object2;
		}
	}
	return NULL;
}

bool CollisionSystem::removeCollider(Collider * col)
{
	for (int i = 0; i < (int)collidersInScene.size(); i++) {
		if (collidersInScene[i] == col) {
			// Swaps the element with the matching name with the last one in the vector and then pops the vector
			iter_swap(collidersInScene.begin() + i, collidersInScene.begin() + collidersInScene.size() - 1);
			collidersInScene.pop_back();
			return true;
		}
	}
	//cout << "COULD NOT REMOVE COLLIDER" << endl;
	return false;
}

