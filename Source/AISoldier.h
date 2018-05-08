#pragma once
#include "AIComponent.h"
#include "PlayerAbilities.h"

class AISoldier : public AIComponent {

private:
	
	SDL_Renderer* renderer;
	Graphics* graphics;
	SDL_Texture* slashAttackIMG; // generalize this to some sort of Attack struct
	
	Entity* slashEntity;
	Animator* slashAnimator;
	Collider* slashCollider;
	Transform* slashTransform = NULL;

	void slashAttack();
	void slashUpdates(float deltaTime);
public:
	AISoldier(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~AISoldier();
	void update(float deltaTime);
	bool slashing;
	enum subStates {
		NONE,
		FINDING,
		SLASHING
	};
	subStates subState = NONE;

	
};
