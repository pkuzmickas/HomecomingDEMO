#include "AISoldier.h"

AISoldier::AISoldier(Entity * owner, SDL_Renderer* renderer, Graphics* graphics) : AIComponent(owner) {
	this->renderer = renderer;
	this->graphics = graphics;
	slashAttackIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "enemySlash.png");

}

AISoldier::~AISoldier() {
	SDL_DestroyTexture(slashAttackIMG);
	if (slashing) {
		graphics->removeFromDraw(slashEntity);
		CollisionSystem::removeCollider(slashCollider);
		delete slashEntity;
	}
}

void AISoldier::update(float deltaTime) {
	if (slashing && stats->curHealth<=0) {
		slashing = false;
		CollisionSystem::removeCollider(slashCollider);
		graphics->removeFromDraw(slashEntity);
		delete slashEntity;
		slashEntity = NULL;
	}
	AIComponent::update(deltaTime);

	if (state == NORMAL) {
		if (slashing) {
			slashCollider->enabled = true;
			CollisionSystem::removeCollider(slashCollider);
			slashing = false;
			graphics->removeFromDraw(slashEntity);
			delete slashEntity;
			slashEntity = NULL;
		}
	}
	if (state == ATTACKING) {
		int chaseSpeed = 200;
		if (subState == NONE) {
			subState = FINDING;
			if (!isKnocked()) {
				walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
			}
		}
		if (subState == FINDING) {
			if (!walking && !isKnocked()) {
				walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
			}
			if (curPathIndex <= path.size() - 3) {
				calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			}
			if (path.size() <= 3) {
				stopWalking();
				subState = SLASHING;
			}
		}
		if (subState == SLASHING) {
			calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			if (path.size() > 3) {
				subState = NONE;
			}
			if (!slashing) {
				slashAttack();
			}
		}
	}

	if (slashing) {
		slashUpdates(deltaTime);
	}
}

void AISoldier::slashAttack() {
	slashEntity = new Entity();
	int width, height;
	if (walkingDir == Animator::LookDirection::DOWN || walkingDir == Animator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (walkingDir == Animator::LookDirection::UP) {
		slashTransform = new Transform(slashEntity, width, height, transform->globalPosX + transform->width / 2 - width / 2, transform->globalPosY - height * 2 / 3);
	}
	if (walkingDir == Animator::LookDirection::DOWN) {
		slashTransform = new Transform(slashEntity, width, height, transform->globalPosX + transform->width / 2 - width / 2, transform->globalPosY + transform->height);
	}
	if (walkingDir == Animator::LookDirection::LEFT) {
		slashTransform = new Transform(slashEntity, width, height, transform->globalPosX - width, transform->globalPosY + transform->height / 2 - width * 2 / 3);
	}
	if (walkingDir == Animator::LookDirection::RIGHT) {
		slashTransform = new Transform(slashEntity, width, height, transform->globalPosX + transform->width, transform->globalPosY + transform->height / 2 - width * 2 / 3);
	}
	slashEntity->addComponent(slashTransform);
	SDL_Rect* src = new SDL_Rect();
	src->h = height;
	src->w = width;
	src->x = 0;
	src->y = walkingDir * src->h;
	if (walkingDir == Animator::LookDirection::UP) {
		src->y = walkingDir * 90 + 90 - src->h;
	}
	Drawable* drawable = new Drawable(slashEntity, slashAttackIMG, "slashAttack", Globals::PLAYER, src);
	slashEntity->addComponent(drawable);

	slashAnimator = new Animator(slashEntity);
	int slashSpeed = 30;
	// playerAnimator->direction * 4 + 0 + 1 + 2 + 3
	int dir = walkingDir * 4;
	if (walkingDir == Animator::LookDirection::UP) {
		dir = 24;
	}
	Animator::Animation slashingAnim("slashing", { dir + 0, dir + 1, dir + 2, dir + 3 }, slashSpeed, 4);
	slashAnimator->addAnimation(slashingAnim);
	slashEntity->addComponent(slashAnimator);
	slashAnimator->playAnimation("slashing");

	slashCollider = new Collider(slashEntity, Collider::ColliderType::NORMAL);
	CollisionSystem::collidersInScene.push_back(slashCollider);
	slashEntity->addComponent(slashCollider);

	slashing = true;
	graphics->addToDraw(slashEntity);
}

void AISoldier::slashUpdates(float deltaTime) {
	slashEntity->update(deltaTime);

	int width, height;
	if (walkingDir == Animator::LookDirection::DOWN || walkingDir == Animator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (walkingDir == Animator::LookDirection::UP) {
		slashTransform->globalPosX = transform->globalPosX + transform->width / 2 - width / 2;
		slashTransform->globalPosY = transform->globalPosY - height * 2 / 3;
	}
	if (walkingDir == Animator::LookDirection::DOWN) {
		slashTransform->globalPosX = transform->globalPosX + transform->width / 2 - width / 2;
		slashTransform->globalPosY = transform->globalPosY + transform->height;
	}
	if (walkingDir == Animator::LookDirection::LEFT) {
		slashTransform->globalPosX = transform->globalPosX - width;
		slashTransform->globalPosY = transform->globalPosY + transform->height / 2 - width * 2 / 3;
	}
	if (walkingDir == Animator::LookDirection::RIGHT) {
		slashTransform->globalPosX = transform->globalPosX + transform->width;
		slashTransform->globalPosY = transform->globalPosY + transform->height / 2 - width * 2 / 3;
	}

	Collider* slashCollision = CollisionSystem::isCollidingWithObjects(slashCollider, {});
	if (slashCollider->enabled && slashCollision) {
		Drawable* draw = (Drawable*)slashCollision->owner->findComponent(ComponentType::DRAWABLE);
		Transform* trans = (Transform*)slashCollision->owner->findComponent(ComponentType::TRANSFORM);
		if (draw->ID == "player") { // can use substrings to know type (soldier)
			slashCollider->enabled = false;
			Stats* enemyStats = (Stats*)owner->findComponent(ComponentType::STATS);
			PlayerStats* playerStats = (PlayerStats*)slashCollision->owner->findComponent(ComponentType::STATS);
			playerStats->curHealth -= enemyStats->mainAttackDmg;
			Drawable* slashDrawable = (Drawable*)slashEntity->findComponent(ComponentType::DRAWABLE);
			//ai->knockBack(100, 500, walkingDir, slashDrawable->ID);
			PlayerAbilities* pa = (PlayerAbilities*)slashCollision->owner->findComponent(ComponentType::ABILITIES);
			pa->knockBack(100, 300, (Animator::LookDirection)walkingDir, "slashAttack");
		}


	}

	if (!slashAnimator->isAnimating()) {
		slashCollider->enabled = true;
		CollisionSystem::removeCollider(slashCollider);
		slashing = false;
		graphics->removeFromDraw(slashEntity);
		delete slashEntity;
		slashEntity = NULL;
	}
}



