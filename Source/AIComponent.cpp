#include "AIComponent.h"
#include <string>
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
		start.coord.y = (transform->globalPosY + transform->height - Globals::TILE_SIZE) / Globals::TILE_SIZE;
		PathfindingSystem::Node goal;
		goal.coord.x = destX / Globals::TILE_SIZE;
		goal.coord.y = (destY + transform->height - Globals::TILE_SIZE) / Globals::TILE_SIZE;
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
			int walkingDir = 2;
			int mapCoordX = (int)transform->globalPosX / Globals::TILE_SIZE;
			int mapCoordY = ((int)transform->globalPosY + transform->height - Globals::TILE_SIZE) / Globals::TILE_SIZE;
			if (path[curPathIndex].x > mapCoordX) {
				walkingDir = (int)Animator::LookDirection::RIGHT;
			}
			if (path[curPathIndex].x < mapCoordX) {
				walkingDir = (int)Animator::LookDirection::LEFT;
			}
			if (path[curPathIndex].y > mapCoordY) {
				walkingDir = (int)Animator::LookDirection::DOWN;
			}
			if (path[curPathIndex].y < mapCoordY) {
				walkingDir = (int)Animator::LookDirection::UP;
			}
			std::string animName = "walking" + std::to_string(walkingDir);
			if (animator->curAnimName() != animName) {
				animator->playAnimation(animName, true);
			}
			if (mapCoordX < path[curPathIndex].x) {
				movement->velX = walkingSpeed;
			}
			if (mapCoordX > path[curPathIndex].x) {
				movement->velX = -walkingSpeed;
			}
			if (mapCoordY < path[curPathIndex].y) {
				movement->velY = walkingSpeed;
			}
			if (mapCoordY > path[curPathIndex].y) {
				movement->velY = -walkingSpeed;
			}
			if (mapCoordX == path[curPathIndex].x && mapCoordY == path[curPathIndex].y) {
				curPathIndex--;
				if (curPathIndex < 0) {
					walking = false;
					movement->velX = 0;
					movement->velY = 0;
					Drawable* drawable = (Drawable*)transform->owner->findComponent(ComponentType::DRAWABLE);
					drawable->srcRect->y = walkingDir * drawable->srcRect->h;
					drawable->srcRect->x = 1 * drawable->srcRect->w;
					animator->stopAnimation();
				}
			}
		}
	}
	else {
		if (walking) walking = false;
	}
}
