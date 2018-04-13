#include "AIComponent.h"

AIComponent::AIComponent(Entity * owner) : Component(owner, true) {
	movement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
	animator = (Animator*)owner->findComponent(ComponentType::ANIMATOR);
	transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	type = AI;
}

void AIComponent::walkTo(int destX, int destY, int walkingSpeed) {
	if (state != ATTACKING) {
		this->walkingSpeed = walkingSpeed;
		this->destX = destX;
		this->destY = destY;
		walking = true;
		PathfindingSystem::Node start;
		start.coord.x = transform->globalPosX / Globals::TILE_SIZE;
		start.coord.y = transform->globalPosY / Globals::TILE_SIZE;
		PathfindingSystem::Node goal;
		goal.coord.x = destX / Globals::TILE_SIZE;
		goal.coord.y = destY / Globals::TILE_SIZE;
		path = PathfindingSystem::AStar(start, goal);
		curPathIndex = path.size() - 1;
	}
	else {
		std::cout << "Cannot walk, currently attacking!" << std::endl;
	}
}

void AIComponent::update(float deltaTime) {
	if (state == NORMAL) {
		if (walking) {
			std::cout << "WALKING! curpathindex:" <<curPathIndex<< std::endl;
			if ((int)transform->globalPosX / Globals::TILE_SIZE < path[curPathIndex].x) {
				movement->velX = walkingSpeed;
			}
			if ((int)transform->globalPosX / Globals::TILE_SIZE > path[curPathIndex].x) {
				movement->velX = -walkingSpeed;
			}
			if ((int)transform->globalPosY / Globals::TILE_SIZE < path[curPathIndex].y) {
				movement->velY = walkingSpeed;
			}
			if ((int)transform->globalPosY / Globals::TILE_SIZE > path[curPathIndex].y) {
				movement->velY = -walkingSpeed;
			}
			if ((int)transform->globalPosX / Globals::TILE_SIZE == path[curPathIndex].x && (int)transform->globalPosY / Globals::TILE_SIZE == path[curPathIndex].y) {
				curPathIndex--;
				if (curPathIndex < 0) {
					walking = false;
					movement->velX = 0;
					movement->velY = 0;
				}
			}
		}
	}
	else {
		if (walking) walking = false;
	}
}
