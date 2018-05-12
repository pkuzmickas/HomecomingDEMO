#pragma once
#include "AIComponent.h"

class AIBoss : public AIComponent {
private:

	SDL_Renderer * renderer;
	Graphics* graphics;
	SDL_Texture* slashAttackIMG; 
	SDL_Texture* smashAttackIMG;


	int slashCooldown = 1500; //in miliseconds
	int smashCooldown = 1000;
	int lastSlashAttack = 0;

	Entity* smashEntity;
	Animator* smashAnimator;
	Collider* smashCollider;
	Transform* smashTransform;
	bool smashing = false;

	void slashUpdates(float deltaTime);
	void smashUpdates(float deltaTime);
public:
	AIBoss(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~AIBoss();
	void update(float deltaTime);
	enum subStates {
		NONE,
		FINDING,
		SLASHING,
		SMASHING
	};
	struct SlashObject {
		Entity* entity;
		Animator* animator;
		Collider* collider;
		Transform* transform;
		Movement* movement;
		Animator::LookDirection slashDir;
	};

	vector<SlashObject> slashPool;
	vector<SlashObject> slashesInUse;

	subStates subState = NONE;
	void slashAttack(int localPosX, int localPosY);
	

	void smashAttack();
	


};
