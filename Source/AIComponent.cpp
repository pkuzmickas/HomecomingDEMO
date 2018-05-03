#include "AIComponent.h"
#include <string>
AIComponent::AIComponent(Entity * owner) : Component(owner, true) {
	movement = (Movement*)owner->findComponent(ComponentType::MOVEMENT);
	animator = (Animator*)owner->findComponent(ComponentType::ANIMATOR);
	transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	collider = (Collider*)owner->findComponent(ComponentType::COLLIDER);
	stats = (Stats*)owner->findComponent(ComponentType::STATS);
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
	if (stats && stats->curHealth <= 0 && state != DEAD) {
		state = DEAD;
		Collider* col = (Collider*)owner->findComponent(ComponentType::COLLIDER);
		col->enabled = false;
		CollisionSystem::removeCollider(col);
		/*Animator* anim = (Animator*)owner->findComponent(ComponentType::ANIMATOR);
		anim->playAnimation("walking0");*/
		owner->active = false;
		UIDesignSystem::removeHealth(owner);
		UIDesignSystem::createBloodshot(owner);
	}
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

	if (isKnocked()) {
		Collider* didCollide = CollisionSystem::isCollidingWithObjects(collider, { knockedByAttackName });
		if (didCollide) {
			knocked = false;
			movement->velX = 0;
			movement->velY = 0;
		}
		if (knockDir == Animator::LookDirection::LEFT) {
			if (transform->globalPosX <= result) {
				knocked = false;
				movement->velX = 0;
			}
		}
		if (knockDir == Animator::LookDirection::RIGHT) {
			if (transform->globalPosX >= result) {
				knocked = false;
				movement->velX = 0;
			}
		}
		if (knockDir == Animator::LookDirection::UP) {
			if (transform->globalPosY <= result) {
				knocked = false;
				movement->velY = 0;
			}
		}
		if (knockDir == Animator::LookDirection::DOWN) {
			if (transform->globalPosY >= result) {
				knocked = false;
				movement->velY = 0;
			}
		}
		if (!knocked) {
			collider->offset.x = 0;
			collider->offset.y = 0;
		}

	}
}

void AIComponent::knockBack(int dist, int speed, Animator::LookDirection dir, std::string attackName) {
	knockedByAttackName = attackName;
	knocked = true;
	knockDir = dir;
	if (!UIDesignSystem::isHealthShowing(owner)) {
		UIDesignSystem::showHealth(owner);
	}
	int colBoxChangeVal = 10; // Changing the collision box a bit to test whether or not it will collide so to not get "stuck"
	if (dir == Animator::LookDirection::LEFT) {
		result = transform->globalPosX - dist;
		movement->velX -= speed;
		collider->offset.x -= colBoxChangeVal;
	}
	if (dir == Animator::LookDirection::RIGHT) {
		result = transform->globalPosX + dist;
		movement->velX += speed;
		collider->offset.x += colBoxChangeVal;
	}
	if (dir == Animator::LookDirection::UP) {
		result = transform->globalPosY - dist;
		movement->velY -= speed;
		collider->offset.y -= colBoxChangeVal;
	}
	if (dir == Animator::LookDirection::DOWN) {
		result = transform->globalPosY + dist;
		movement->velY += speed;
		collider->offset.y += colBoxChangeVal;
	}

}
