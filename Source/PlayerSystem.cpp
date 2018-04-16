#include "PlayerSystem.h"
Entity * PlayerSystem::player;

Entity * PlayerSystem::createPlayer(int globalPosX, int globalPosY, SDL_Texture* texture, PlayerAnimator::LookDirection lookDirection) {
	Entity* player = new Entity();
	PlayerSystem::player = player;
	Transform* playerTransform = new Transform(player, 48, 48, globalPosX, globalPosY);
	player->addComponent(playerTransform);
	PlayerStats* playerStats = new PlayerStats(player);
	player->addComponent(playerStats);
	SDL_Rect* srcRect = new SDL_Rect();
	srcRect->h = 48;
	srcRect->w = 48;
	srcRect->x = 0;
	srcRect->y = 0;
	Drawable* playerDrawable = new Drawable(player, texture, "player", Globals::Layers::PLAYER, srcRect);
	player->addComponent(playerDrawable);
	Collider* playerCollider = new Collider(player);
	player->addComponent(playerCollider);
	Movement* playerMovement = new PlayerMovement(player);
	player->addComponent(playerMovement);
	PlayerAnimator* animator = new PlayerAnimator(player, lookDirection);
	player->addComponent(animator);
	int walkAnimSpeed = 150;
	Animator::Animation walkingd("walking0", { 0, 1, 2, 1 }, walkAnimSpeed, false);
	animator->addAnimation(walkingd);
	Animator::Animation walkingl("walking1", { 3, 4, 5, 4 }, walkAnimSpeed, false);
	animator->addAnimation(walkingl);
	Animator::Animation walkingr("walking2", { 6, 7, 8, 7 }, walkAnimSpeed, false);
	animator->addAnimation(walkingr);
	Animator::Animation walkingu("walking3", { 9, 10, 11, 10 }, walkAnimSpeed, false);
	animator->addAnimation(walkingu);
	PlayerInput* playerInput = new PlayerInput(player);
	player->addComponent(playerInput);
	return player;
}

void PlayerSystem::disableMovement() {
	player->findComponent(ComponentType::ANIMATOR)->enabled = false;
	player->findComponent(ComponentType::MOVEMENT)->enabled = false;
}

void PlayerSystem::enableMovement() {
	player->findComponent(ComponentType::MOVEMENT)->enabled = true;
	player->findComponent(ComponentType::ANIMATOR)->enabled = true;
}

Entity * PlayerSystem::getPlayer() {
	return player;
}
