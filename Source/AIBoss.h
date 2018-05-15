#pragma once
#include "AIComponent.h"

class AIBoss : public AIComponent {
private:

	SDL_Renderer * renderer;
	Graphics* graphics;
	SDL_Texture* slashAttackIMG; 
	SDL_Texture* smashAttackIMG;

	// Attack cooldowns
	int slashCooldown = 1500; //in miliseconds
	int smashCooldown = 1000;
	int lastSlashAttack = 0;

	// Global variables needed for the update function and other functions
	Entity* smashEntity;
	Animator* smashAnimator;
	Collider* smashCollider;
	Transform* smashTransform;
	bool smashing = false;

	// Update functions for different attacks
	void slashUpdates(float deltaTime);
	void smashUpdates(float deltaTime);
public:
	AIBoss(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~AIBoss();
	// The main update function
	void update(float deltaTime);

	// Different substates for when in combat (state is ATTACKING)
	enum subStates {
		NONE,
		FINDING,
		SLASHING,
		SMASHING
	};
	
	// OBJECT POOLING 

	// A struct that defines an object that will be inside the object pool
	struct SlashObject {
		Entity* entity;
		Animator* animator;
		Collider* collider;
		Transform* transform;
		Movement* movement;
		Animator::LookDirection slashDir;
	};
	// The actual object pool
	vector<SlashObject> slashPool;
	
	// OBJECT POOLING ^
	
	vector<SlashObject> slashesInUse;

	subStates subState = NONE;
	void slashAttack(int localPosX, int localPosY);
	
	void smashAttack();
	
};
