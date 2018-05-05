#pragma once
#include "AIComponent.h"
#include "PlayerAbilities.h"

class AISoldier : public AIComponent {
public:
	AISoldier(Entity* owner, SDL_Renderer* renderer, Graphics* graphics);
	~AISoldier();
	void update(float deltaTime);
private:
	enum subStates {
		NONE,
		FINDING,
		SLASHING
	};
	SDL_Renderer* renderer;
	Graphics* graphics;
	subStates subState = NONE;
	SDL_Texture* slashAttackIMG; // generalize this to some sort of Attack struct
	bool slashing;
	Entity* slashEntity;
	Animator* slashAnimator;
	Collider* slashCollider;
	Transform* slashTransform = NULL;

	void slashAttack();
	void slashUpdates(float deltaTime);

	
};
