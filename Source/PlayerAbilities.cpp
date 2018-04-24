#include "PlayerAbilities.h"

PlayerAbilities::PlayerAbilities(Entity * owner, SDL_Renderer* renderer, Graphics* graphics) : Abilities(owner) {
	playerTransform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	playerAnimator = (PlayerAnimator*)owner->findComponent(ComponentType::ANIMATOR);
	player = owner;
	this->renderer = renderer;
	this->graphics = graphics;

	slashAttackIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "slash.png");
}


PlayerAbilities::~PlayerAbilities() {
	SDL_DestroyTexture(slashAttackIMG);
}

void PlayerAbilities::slashAttack() {
	slashEntity = new Entity();
	int width, height;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN || playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width / 2 - width / 2, playerTransform->globalPosY - height * 2 / 3);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width / 2 - width / 2, playerTransform->globalPosY + playerTransform->height);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::LEFT) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX - width, playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::RIGHT) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width, playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3);
	}
	slashEntity->addComponent(slashTransform);
	SDL_Rect* src = new SDL_Rect();
	src->h = height;
	src->w = width;
	src->x = 0;
	src->y = playerAnimator->direction * src->h;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		src->y = playerAnimator->direction * 90 + 90 - src->h;
	}
	Drawable* drawable = new Drawable(slashEntity, slashAttackIMG, "slashAttack", Globals::PLAYER, src);
	slashEntity->addComponent(drawable);

	slashAnimator = new Animator(slashEntity);
	int slashSpeed = 30;
	// playerAnimator->direction * 4 + 0 + 1 + 2 + 3
	int dir = playerAnimator->direction * 4;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		dir = 24;
	}
	Animator::Animation slashingAnim("slashing", { dir + 0, dir + 1, dir + 2, dir + 3 }, slashSpeed, false, 4);
	slashAnimator->addAnimation(slashingAnim);
	slashEntity->addComponent(slashAnimator);
	slashAnimator->playAnimation("slashing");

	slashCollider = new Collider(slashEntity, Collider::ColliderType::NORMAL);
	CollisionSystem::collidersInScene.push_back(slashCollider);
	slashEntity->addComponent(slashCollider);

	slashing = true;
	graphics->addToDraw(slashEntity);
}

void PlayerAbilities::update(float deltaTime) {
	if (slashing) {
		slashUpdates(deltaTime);
	}
}

void PlayerAbilities::slashUpdates(float deltaTime) {
	slashEntity->update(deltaTime);

	int width, height;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN || playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		slashTransform->globalPosX = playerTransform->globalPosX + playerTransform->width / 2 - width / 2;
		slashTransform->globalPosY = playerTransform->globalPosY - height * 2 / 3;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN) {
		slashTransform->globalPosX = playerTransform->globalPosX + playerTransform->width / 2 - width / 2;
		slashTransform->globalPosY = playerTransform->globalPosY + playerTransform->height;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::LEFT) {
		slashTransform->globalPosX = playerTransform->globalPosX - width;
		slashTransform->globalPosY = playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::RIGHT) {
		slashTransform->globalPosX = playerTransform->globalPosX + playerTransform->width;
		slashTransform->globalPosY = playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3;
	}

	Collider* slashCollision = CollisionSystem::isCollidingWithObjects(slashCollider, {});
	if (slashCollision) {
		Drawable* draw = (Drawable*)slashCollision->owner->findComponent(ComponentType::DRAWABLE);
		Transform* trans = (Transform*)slashCollision->owner->findComponent(ComponentType::TRANSFORM);
		if (slashCollision->owner->hasComponent(ComponentType::AI)) {
			AIComponent* ai = (AIComponent*)slashCollision->owner->findComponent(ComponentType::AI);
			if (draw->ID == "soldier2") { // can use substrings to know type (soldier)
				ai->knockBack(100, 200);
			}
		}
		
	}

	if (!slashAnimator->isAnimating()) {
		CollisionSystem::removeCollider(slashCollider);
		slashing = false;
		graphics->removeFromDraw(slashEntity);
		delete slashEntity;
		slashEntity = NULL;
	}
}


