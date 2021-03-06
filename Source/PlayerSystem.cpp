#include "PlayerSystem.h"
Entity * PlayerSystem::player;

Entity * PlayerSystem::createPlayer(int globalPosX, int globalPosY, SDL_Texture* texture, SDL_Renderer* renderer, Graphics* graphics, PlayerAnimator::LookDirection lookDirection) {
	Entity* player = new Entity();
	PlayerSystem::player = player;
	Transform* playerTransform = new Transform(player, 48, 48, globalPosX, globalPosY);
	player->addComponent(playerTransform);
	PlayerStats* playerStats = new PlayerStats(player);
	player->addComponent(playerStats);
	playerStats->mainAttackDmg = 30;
	playerStats->totalHealth = 200;
	playerStats->curHealth = 200;
	SDL_Rect* srcRect = new SDL_Rect();
	srcRect->h = 48;
	srcRect->w = 48;
	srcRect->x = 0;
	srcRect->y = 0;
	Drawable* playerDrawable = new Drawable(player, texture, "player", Globals::Layers::PLAYER, srcRect);
	player->addComponent(playerDrawable);
	Collider* playerCollider = new Collider(player);
	player->addComponent(playerCollider);
	CollisionSystem::collidersInScene.push_back(playerCollider);
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
	PlayerAbilities* playerAbilities = new PlayerAbilities(player, renderer, graphics);
	player->addComponent(playerAbilities);
	
	
	PlayerInput* playerInput = new PlayerInput(player);
	player->addComponent(playerInput);
	
	return player;
}

void PlayerSystem::disableMovement() {
	player->findComponent(ComponentType::ANIMATOR)->enabled = false;
	PlayerMovement* pm = (PlayerMovement*)player->findComponent(ComponentType::MOVEMENT);
	pm->movementEnabled = false;
}

void PlayerSystem::enableMovement() {
	PlayerMovement* pm = (PlayerMovement*)player->findComponent(ComponentType::MOVEMENT);
	pm->movementEnabled = true;
	player->findComponent(ComponentType::ANIMATOR)->enabled = true;
}

Entity * PlayerSystem::getPlayer() {
	return player;
}

void PlayerSystem::resetPlayer() {
	PlayerStats* ps = (PlayerStats*)player->findComponent(ComponentType::STATS);
	ps->curHealth = 200;
	ps->totalHealth = 200;
	PlayerAbilities* pa = (PlayerAbilities*)player->findComponent(ComponentType::ABILITIES);
	pa->isAlive = true;
	pa->knocked = false;
	PlayerMovement* pm = (PlayerMovement*)player->findComponent(ComponentType::MOVEMENT);
	pm->velX = 0;
	pm->velY = 0;
	pm->movementEnabled = true;
	
	
	for (auto dir : pm->moving) {
		dir = false;
	}
	Animator* anim = (PlayerAnimator*)player->findComponent(ComponentType::ANIMATOR);
	anim->stopAnimation();
	anim->enabled = true;
	Collider* col = (Collider*)player->findComponent(ComponentType::COLLIDER);
	Transform* transform = (Transform*)player->findComponent(ComponentType::TRANSFORM);
	col->offset.x = 0;
	col->offset.y = 0;
	col->colBox.x = (int)(transform->globalPosX - CameraSystem::posX) + col->offset.x;
	col->colBox.y = (int)(transform->globalPosY - CameraSystem::posY) + col->offset.y;
	col->enabled = true;
}

