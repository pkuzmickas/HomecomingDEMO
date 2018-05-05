#pragma once
#include "Entity.h"
#include <SDL.h>
#include <SDL_image.h>
#include "PlayerAnimator.h"
#include "Graphics.h"
#include "CollisionSystem.h"
#include "AIComponent.h"
#include "PlayerMovement.h"

class PlayerAbilities : public Abilities {
public:
	PlayerAbilities(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~PlayerAbilities();
	void slashAttack();
	void flyingSlashAttack();
	void dashMove(); // add mana to this later
	void update(float deltaTime);
	bool isSlashing() { return slashing; }
	bool isDashing() { return dashing; }
	bool isFSlashing() { return flyingSlashing; }

	bool isKnocked() { return knocked; }
	void knockBack(int dist, int speed, Animator::LookDirection dir, std::string attackName);

private:
	void slashUpdates(float deltaTime);
	void dashUpdates(float deltaTime);
	void fSlashUpdates(float deltaTime);

	Animator::LookDirection knockDir;
	std::string knockedByAttackName = "";
	int result;
	bool knocked;

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
	Transform* dashTransform = NULL;
	int dashStart = 0;

	SDL_Texture* flyingSlashIMG;
	bool flyingSlashing;
	Entity* fSlashEntity;
	Animator* fSlashAnimator;
	Collider* fSlashCollider;
	Transform* fSlashTransform = NULL;
	Movement* fSlashMovement;
	Animator::LookDirection fSlashDir;
	


};