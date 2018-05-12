#pragma once
#include "AIComponent.h"

class AIBoss : public AIComponent {
private:

	SDL_Renderer * renderer;
	Graphics* graphics;
	SDL_Texture* slashAttackIMG; // generalize this to some sort of Attack struct

	int cooldown = 2000; //in miliseconds
	int lastSlashAttack = 0;

	struct SlashObject {
		Entity* entity;
		Animator* animator;
		Collider* collider;
		Transform* transform = NULL;
		Movement* movement;
		Animator::LookDirection slashDir;
	};

	vector<SlashObject> slashPool;

	

	void slashUpdates(float deltaTime);
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
	subStates subState = NONE;
	void slashAttack(int localPosX, int localPosY);
	vector<SlashObject> slashesInUse;
	


};
