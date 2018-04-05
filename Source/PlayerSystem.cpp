#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(SDL_Renderer* renderer) {
	this->renderer = renderer;

}

Entity * PlayerSystem::createPlayer(int globalPosX, int globalPosY) {

	player = new Entity();
	Transform* playerTransform = new Transform(player, 48, 48, globalPosX, globalPosY);
	player->addComponent(playerTransform);
	PlayerStats* playerStats = new PlayerStats(player);
	player->addComponent(playerStats);
	SDL_Rect* srcRect = new SDL_Rect();
	srcRect->h = 48;
	srcRect->w = 48;
	srcRect->x = 0;
	srcRect->y = 0;
	Drawable* playerDrawable = new Drawable(player, IMG_LoadTexture(renderer, ASSET_DIR"player.png"), "player", Globals::Layers::PLAYER, srcRect);
	player->addComponent(playerDrawable);
	Movement* playerMovement = new PlayerMovement(player);
	player->addComponent(playerMovement);
	PlayerInput* playerInput = new PlayerInput(player);
	player->addComponent(playerInput);
	Animator* animator = new Animator(player);
	player->addComponent(animator);
	Animator::Animation walking("walking0", { 0, 1, 2, 1 }, 200, true);
	animator->addAnimation(walking);
	animator->playAnimation("walking0");

	this->player = player;
	return player;
}
