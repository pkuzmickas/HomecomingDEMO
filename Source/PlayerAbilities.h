#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_image.h>
#include "PlayerAnimator.h"
#include "Graphics.h"
#include "CollisionSystem.h"
#include "AIComponent.h"

class PlayerAbilities : public Abilities {
public:
	PlayerAbilities(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~PlayerAbilities();
	void slashAttack();
	void dashMove(); // add mana to this later
	void update(float deltaTime);
	bool isSlashing() { return slashing; }
	bool isDashing() { return dashing; }
private:
	void slashUpdates(float deltaTime);
	void dashUpdates(float deltaTime);
	SDL_Renderer * renderer;
	Graphics* graphics;

	Entity * player;
	Transform * playerTransform;
	PlayerAnimator* playerAnimator;
	
	SDL_Texture* slashAttackIMG; // generalize this to some sort of Attack struct
	bool slashing;
	Entity* slashEntity;
	Animator* slashAnimator;
	Collider* slashCollider;
	Transform* slashTransform = NULL;

	SDL_Texture* dashIMG;
	bool dashing;
	Entity* dashEntity;
	Animator* dashAnimator;
	Collider* dashCollider;
	Transform* dashTransform = NULL;
	int dashStart = 0;


};