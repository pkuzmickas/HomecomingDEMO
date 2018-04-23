#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_image.h>
#include "PlayerAnimator.h"
#include "Graphics.h"
class PlayerAbilities : public Abilities {
public:
	PlayerAbilities(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~PlayerAbilities();
	void slashAttack();
	void update(float deltaTime);
	bool isSlashing() { return slashing; }
private:
	void initSlash();

	SDL_Renderer * renderer;
	Graphics* graphics;

	Entity * player;
	Transform * playerTransform;
	PlayerAnimator* playerAnimator;
	
	SDL_Texture* slashAttackIMG;
	bool slashing;
	Entity* slashEntity;
	Animator* slashAnimator;

};