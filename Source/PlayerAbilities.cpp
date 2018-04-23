#include "PlayerAbilities.h"

PlayerAbilities::PlayerAbilities(Entity * owner, SDL_Renderer* renderer, Graphics* graphics) : Abilities(owner) {
	playerTransform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	playerAnimator = (PlayerAnimator*)owner->findComponent(ComponentType::ANIMATOR);
	player = owner;
	this->renderer = renderer;
	this->graphics = graphics;

	initSlash();
}

void PlayerAbilities::initSlash() {
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
	Transform* transform = NULL;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		transform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width / 2 - width / 2, playerTransform->globalPosY - height*2/3);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN) {
		transform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width / 2 - width / 2, playerTransform->globalPosY + playerTransform->height);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::LEFT) {
		transform = new Transform(slashEntity, width, height, playerTransform->globalPosX - width, playerTransform->globalPosY + playerTransform->height/2 - width*2/3);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::RIGHT) {
		transform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width, playerTransform->globalPosY + playerTransform->height / 2 - width*2/3);
	}
	slashEntity->addComponent(transform);
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
	int slashSpeed = 40;
	// playerAnimator->direction * 4 + 0 + 1 + 2 + 3
	int dir = playerAnimator->direction * 4;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		dir = 24;
	}
	Animator::Animation slashingAnim("slashing", { dir + 0, dir + 1, dir + 2, dir + 3 }, slashSpeed, false, 4);
	slashAnimator->addAnimation(slashingAnim);
	slashEntity->addComponent(slashAnimator);

	slashAnimator->playAnimation("slashing");

	slashing = true;
	graphics->addToDraw(slashEntity);
}

void PlayerAbilities::update(float deltaTime) {
	if (slashing) {
		slashEntity->update(deltaTime);
	}
	if (slashing && !slashAnimator->isAnimating()) {
		slashing = false;
		graphics->removeFromDraw(slashEntity);
		delete slashEntity;
		slashEntity = NULL;

	}
}


