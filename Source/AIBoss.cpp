#include "AIBoss.h"

AIBoss::AIBoss(Entity * owner, SDL_Renderer * renderer, Graphics * graphics) : AIComponent(owner) {
	this->renderer = renderer;
	this->graphics = graphics;
	slashAttackIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "enemyFlyingSlash.png");
	smashAttackIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "enemySlash.png");

	for (int i = 0; i < 3; i++) {
		SlashObject so;
		so.entity = new Entity();
		int width;
		SDL_QueryTexture(slashAttackIMG, NULL, NULL, &width, NULL);
		int height = 90;
		so.transform = new Transform(so.entity, width * 2, height * 2, transform->globalPosX, transform->globalPosY);
		so.entity->addComponent(so.transform);
		SDL_Rect* src = new SDL_Rect();
		src->h = height;
		src->w = width;
		src->x = 0;
		src->y = 0;
		Drawable* slashDrawable = new Drawable(so.entity, slashAttackIMG, "enemyfslash", Globals::Layers::PLAYER, src);
		so.entity->addComponent(slashDrawable);
		so.animator = new Animator(so.entity);
		int slashAnimSpeed = 60;

		Animator::Animation slashStart("fSlashStart", { 0, 1, 2 }, slashAnimSpeed, false);
		so.animator->addAnimation(slashStart);
		Animator::Animation fslashing("fSlashing", { 1, 2 }, slashAnimSpeed, true);
		so.animator->addAnimation(fslashing);
		so.entity->addComponent(so.animator);
		so.animator->playAnimation("fSlashStart");
		so.collider = new Collider(so.entity, Collider::NORMAL, 0, 20, 50, 50);
		so.entity->addComponent(so.collider);
		so.movement = new Movement(so.entity);
		so.entity->addComponent(so.movement);

		slashPool.push_back(so);
	}
}

void AIBoss::slashAttack(int localPosX, int localPosY) {
	if (slashPool.size() == 0) {
		return;
	}
	float dx = localPosX - (transform->globalPosX + transform->width / 2 - CameraSystem::posX);
	float dy = localPosY - (transform->globalPosY + transform->height / 2 - CameraSystem::posY);
	float angle = atan2(dy, dx) * 180 / M_PI;
	SlashObject so = slashPool[slashPool.size() - 1];
	so.transform->globalPosX = transform->globalPosX + 20 * cos(angle * M_PI / 180);
	so.transform->globalPosY = transform->globalPosY - transform->height / 2 + 20 * sin(angle * M_PI / 180);
	int width;
	SDL_QueryTexture(slashAttackIMG, NULL, NULL, &width, NULL);
	int height = 90;
	SDL_Point center;
	center.y = height / 2;
	center.x = width / 2;
	so.transform->rotate(angle, &center);
	CollisionSystem::collidersInScene.push_back(so.collider);
	float slashFlySpeed = 300;
	so.movement->velX = slashFlySpeed * cos(angle * M_PI / 180);
	so.movement->velY = slashFlySpeed * sin(angle * M_PI / 180);
	graphics->addToDraw(so.entity);
	so.slashDir = (Animator::LookDirection)walkingDir;
	slashPool.pop_back();
	slashesInUse.push_back(so);
}

void AIBoss::smashAttack() {
	smashEntity = new Entity();
	int width, height;
	if (walkingDir == Animator::LookDirection::DOWN || walkingDir == Animator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (walkingDir == Animator::LookDirection::UP) {
		smashTransform = new Transform(smashEntity, width, height, transform->globalPosX + transform->width / 2 - width / 2, transform->globalPosY - height * 2 / 3);
	}
	if (walkingDir == Animator::LookDirection::DOWN) {
		smashTransform = new Transform(smashEntity, width, height, transform->globalPosX + transform->width / 2 - width / 2, transform->globalPosY + transform->height);
	}
	if (walkingDir == Animator::LookDirection::LEFT) {
		smashTransform = new Transform(smashEntity, width, height, transform->globalPosX - width, transform->globalPosY + transform->height / 2 - width * 2 / 3);
	}
	if (walkingDir == Animator::LookDirection::RIGHT) {
		smashTransform = new Transform(smashEntity, width, height, transform->globalPosX + transform->width, transform->globalPosY + transform->height / 2 - width * 2 / 3);
	}
	smashEntity->addComponent(smashTransform);
	SDL_Rect* src = new SDL_Rect();
	src->h = height;
	src->w = width;
	src->x = 0;
	src->y = walkingDir * src->h;
	if (walkingDir == Animator::LookDirection::UP) {
		src->y = walkingDir * 90 + 90 - src->h;
	}
	Drawable* drawable = new Drawable(smashEntity, smashAttackIMG, "smashAttack", Globals::PLAYER, src);
	smashEntity->addComponent(drawable);

	smashAnimator = new Animator(smashEntity);
	int slashSpeed = 30;
	// playerAnimator->direction * 4 + 0 + 1 + 2 + 3
	int dir = walkingDir * 4;
	if (walkingDir == Animator::LookDirection::UP) {
		dir = 24;
	}
	Animator::Animation slashingAnim("slashing", { dir + 0, dir + 1, dir + 2, dir + 3 }, slashSpeed, 4);
	smashAnimator->addAnimation(slashingAnim);
	smashEntity->addComponent(smashAnimator);
	smashAnimator->playAnimation("slashing");

	smashCollider = new Collider(smashEntity, Collider::ColliderType::NORMAL);
	CollisionSystem::collidersInScene.push_back(smashCollider);
	smashEntity->addComponent(smashCollider);

	smashing = true;
	graphics->addToDraw(smashEntity);
}

void AIBoss::slashUpdates(float deltaTime) {
	for(int i=0; i<(int)slashesInUse.size(); i++) {
		SlashObject slash = slashesInUse[i];
		slash.entity->update(deltaTime);
		if (!slash.animator->isAnimating()) {
			slash.animator->playAnimation("fSlashing", true);
		}


		Collider* collision1 = CollisionSystem::isCollidingWithObjects(slash.collider, { });
		Collider* collision2 = CollisionSystem::isCollidingWithEnv(slash.collider);
		if (collision2 || collision1) {
			if (collision1) {
				Drawable* colDrw = (Drawable*)collision1->owner->findComponent(ComponentType::DRAWABLE);
				/*if (colDrw->ID == "soldier1" || colDrw->ID == "soldier2" || colDrw->ID == "oldman") {
					AIComponent* ai = (AIComponent*)collision->owner->findComponent(ComponentType::AI);
					Stats* enemyStats = (Stats*)collision->owner->findComponent(ComponentType::STATS);
					PlayerStats* playerStats = (PlayerStats*)->findComponent(ComponentType::STATS);
					enemyStats->curHealth -= playerStats->fSlashAttackDmg;
					Drawable* fslashDrawable = (Drawable*)fSlashEntity->findComponent(ComponentType::DRAWABLE);
					ai->knockBack(200, 500, fSlashDir, fslashDrawable->ID);
				}*/
				if (colDrw->ID == "tree") {
					Animator* anim = (Animator*)collision1->owner->findComponent(ComponentType::ANIMATOR);
					Stats* treeStats = (Stats*)collision1->owner->findComponent(ComponentType::STATS);
					treeStats->curHealth -= 50;
					if (treeStats->curHealth == 0) {
						anim->playAnimation("treeHit");
						Collider* col = (Collider*)collision1->owner->findComponent(ComponentType::COLLIDER);
						col->enabled = false;
						CollisionSystem::removeCollider(col);
						collision1->owner->active = false;
					}
					else {
						anim->playAnimation("treeHit2");
						collision1->owner->active = false;
					}
				}
				if (colDrw->ID == "player") { // can use substrings to know type (soldier)
					CollisionSystem::removeCollider(slash.collider);
					graphics->removeFromDraw(slash.entity);
					iter_swap(slashesInUse.begin() + i, slashesInUse.begin() + slashesInUse.size() - 1);
					slashPool.push_back(slashesInUse[slashesInUse.size() - 1]);
					slashesInUse.pop_back();
					Stats* enemyStats = (Stats*)owner->findComponent(ComponentType::STATS);
					PlayerStats* playerStats = (PlayerStats*)colDrw->owner->findComponent(ComponentType::STATS);
					playerStats->curHealth -= enemyStats->mainAttackDmg;
					Drawable* slashDrawable = (Drawable*)slash.entity->findComponent(ComponentType::DRAWABLE);
					PlayerAbilities* pa = (PlayerAbilities*)colDrw->owner->findComponent(ComponentType::ABILITIES);
					pa->knockBack(100, 300, (Animator::LookDirection)walkingDir, "slashAttack");
				}
			}
			if (collision2) {
				CollisionSystem::removeCollider(slash.collider);
				graphics->removeFromDraw(slash.entity);
				iter_swap(slashesInUse.begin() + i, slashesInUse.begin() + slashesInUse.size() - 1);
				slashPool.push_back(slashesInUse[slashesInUse.size() - 1]);
				slashesInUse.pop_back();


			}
		}
	}
}

void AIBoss::smashUpdates(float deltaTime) {
	smashEntity->update(deltaTime);

	int width, height;
	if (walkingDir == Animator::LookDirection::DOWN || walkingDir == Animator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (walkingDir == Animator::LookDirection::UP) {
		smashTransform->globalPosX = transform->globalPosX + transform->width / 2 - width / 2;
		smashTransform->globalPosY = transform->globalPosY - height * 2 / 3;
	}
	if (walkingDir == Animator::LookDirection::DOWN) {
		smashTransform->globalPosX = transform->globalPosX + transform->width / 2 - width / 2;
		smashTransform->globalPosY = transform->globalPosY + transform->height;
	}
	if (walkingDir == Animator::LookDirection::LEFT) {
		smashTransform->globalPosX = transform->globalPosX - width;
		smashTransform->globalPosY = transform->globalPosY + transform->height / 2 - width * 2 / 3;
	}
	if (walkingDir == Animator::LookDirection::RIGHT) {
		smashTransform->globalPosX = transform->globalPosX + transform->width;
		smashTransform->globalPosY = transform->globalPosY + transform->height / 2 - width * 2 / 3;
	}

	Collider* smashCollision = CollisionSystem::isCollidingWithObjects(smashCollider, {"slashAttack"});
	if (smashCollider->enabled && smashCollision) {
		Drawable* draw = (Drawable*)smashCollision->owner->findComponent(ComponentType::DRAWABLE);
		Transform* trans = (Transform*)smashCollision->owner->findComponent(ComponentType::TRANSFORM);
		if (draw->ID == "player") { // can use substrings to know type (soldier)
			smashCollider->enabled = false;
			Stats* enemyStats = (Stats*)owner->findComponent(ComponentType::STATS);
			PlayerStats* playerStats = (PlayerStats*)smashCollision->owner->findComponent(ComponentType::STATS);
			playerStats->curHealth -= enemyStats->secondAttackDmg;
			Drawable* slashDrawable = (Drawable*)smashEntity->findComponent(ComponentType::DRAWABLE);
			//ai->knockBack(100, 500, walkingDir, slashDrawable->ID);
			PlayerAbilities* pa = (PlayerAbilities*)smashCollision->owner->findComponent(ComponentType::ABILITIES);
			pa->knockBack(500, 800, (Animator::LookDirection)walkingDir, "smashAttack");
		}


	}

	if (!smashAnimator->isAnimating()) {
		smashCollider->enabled = true;
		CollisionSystem::removeCollider(smashCollider);
		smashing = false;
		graphics->removeFromDraw(smashEntity);
		delete smashEntity;
		smashEntity = NULL;
	}
}



AIBoss::~AIBoss() {
	SDL_DestroyTexture(slashAttackIMG);
	SDL_DestroyTexture(smashAttackIMG);
	for (auto slash : slashPool) {
		delete slash.entity;
	}
	for (auto slash : slashesInUse) {
		delete slash.entity;
	}
}

void AIBoss::update(float deltaTime) {
	AIComponent::update(deltaTime);
	if (slashesInUse.size()>0) {
		slashUpdates(deltaTime);
	}
	if (smashing) {
		smashUpdates(deltaTime);
	}
	if (state == DEAD) {
		if (smashing) {

			smashCollider->enabled = true;
			CollisionSystem::removeCollider(smashCollider);
			smashing = false;
			graphics->removeFromDraw(smashEntity);
			delete smashEntity;
			smashEntity = NULL;
		}
		for (auto slash : slashesInUse) {
			graphics->removeFromDraw(slash.entity);
			CollisionSystem::removeCollider(slash.collider);
		}
	}
	if (state == NORMAL) {
		
		if (smashing) {

			smashCollider->enabled = true;
			CollisionSystem::removeCollider(smashCollider);
			smashing = false;
			graphics->removeFromDraw(smashEntity);
			delete smashEntity;
			smashEntity = NULL;
		}
	}
	if (state == ATTACKING) {
		int chaseSpeed = stats->speed;
		if (subState == NONE) {
			stopWalking();
			subState = FINDING;
			if (!isKnocked()) {
				walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
			}
		}
		if (subState == FINDING) {
			if (!walking && !isKnocked()) {
				walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
			}
			if (curPathIndex <= path.size() - 6) {
				calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			}
			if (path.size() <= 16) {
				stopWalking();
				subState = SLASHING;
			}
		}
		if (subState == SLASHING) {
			if (!walking || curPathIndex <= path.size() - 3) {
				calculatePath(targetTransform->globalPosX, targetTransform->globalPosY);
			}
			
			if (path.size() > 16) {
				subState = NONE;
			}
			if (path.size() > 10) {
				if (SDL_GetTicks() > slashCooldown + lastSlashAttack) {
					if (slashPool.size() > 0) {
						lastSlashAttack = SDL_GetTicks();
					} 
					slashAttack(targetTransform->globalPosX - CameraSystem::posX + targetTransform->width / 2, targetTransform->globalPosY - CameraSystem::posY + targetTransform->height / 2);


				}
			}
			else if (path.size() > 3 && !walking && !knocked) {
				walkTo(targetTransform->globalPosX, targetTransform->globalPosY, chaseSpeed);
			}
			else if(path.size() <= 3) {
				stopWalking();
				if (!smashing && lastSlashAttack + smashCooldown < SDL_GetTicks()) {
					smashAttack();
					lastSlashAttack = SDL_GetTicks();
				}
			}
			
		}
	}

}
