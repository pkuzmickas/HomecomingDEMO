#include "AIBoss.h"

AIBoss::AIBoss(Entity * owner, SDL_Renderer * renderer, Graphics * graphics) : AIComponent(owner) {
	this->renderer = renderer;
	this->graphics = graphics;
	slashAttackIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "enemyFlyingSlash.png");

	// Creates the slash object pool with 3 objects
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
	if (slashPool.size() == 0) return;
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



AIBoss::~AIBoss() {
	SDL_DestroyTexture(slashAttackIMG);
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
}
