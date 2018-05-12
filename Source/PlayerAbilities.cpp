#include "PlayerAbilities.h"

PlayerAbilities::PlayerAbilities(Entity * owner, SDL_Renderer* renderer, Graphics* graphics) : Abilities(owner) {
	playerTransform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
	playerAnimator = (PlayerAnimator*)owner->findComponent(ComponentType::ANIMATOR);
	playerStats = (PlayerStats*)owner->findComponent(ComponentType::STATS);
	player = owner;
	this->renderer = renderer;
	this->graphics = graphics;

	slashAttackIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "slash.png");
	dashIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "dash.png");
	flyingSlashIMG = IMG_LoadTexture(renderer, ASSET_DIR ATTACKS_DIR "flyingSlash.png");
}


PlayerAbilities::~PlayerAbilities() {
	SDL_DestroyTexture(slashAttackIMG);
	SDL_DestroyTexture(dashIMG);
	SDL_DestroyTexture(flyingSlashIMG);
}

void PlayerAbilities::flyingSlashAttack() {
	if (flyingSlashing) return;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	fSlashEntity = new Entity();
	int width;
	SDL_QueryTexture(flyingSlashIMG, NULL, NULL, &width, NULL);
	int height = 90;
	float dx = mouseX - (playerTransform->globalPosX + playerTransform->width / 2 - CameraSystem::posX);
	float dy = mouseY - (playerTransform->globalPosY + playerTransform->height / 2 - CameraSystem::posY);
	float angle = atan2(dy, dx) * 180 / M_PI;
	fSlashTransform = new Transform(fSlashEntity, width, height, playerTransform->globalPosX + 20 * cos(angle * M_PI / 180), playerTransform->globalPosY - playerTransform->height / 2 + 20 * sin(angle * M_PI / 180));
	SDL_Point center;
	center.y = height / 2;
	center.x = width / 2;
	fSlashTransform->rotate(angle, &center);
	fSlashEntity->addComponent(fSlashTransform);
	SDL_Rect* src = new SDL_Rect();
	src->h = height;
	src->w = width;
	src->x = 0;
	src->y = 0;
	Drawable* fSlashDrawable = new Drawable(fSlashEntity, flyingSlashIMG, "fslash", Globals::Layers::PLAYER, src);
	fSlashEntity->addComponent(fSlashDrawable);
	fSlashAnimator = new Animator(fSlashEntity);
	int slashAnimSpeed = 60;
	float slashFlySpeed = 0.6f;
	Animator::Animation slashStart("fSlashStart", { 0, 1, 2 }, slashAnimSpeed, false);
	fSlashAnimator->addAnimation(slashStart);
	Animator::Animation fslashing("fSlashing", { 1, 2 }, slashAnimSpeed, true);
	fSlashAnimator->addAnimation(fslashing);
	fSlashEntity->addComponent(fSlashAnimator);
	fSlashAnimator->playAnimation("fSlashStart");
	fSlashCollider = new Collider(fSlashEntity, Collider::NORMAL, 0, 20, 50, 50);
	CollisionSystem::collidersInScene.push_back(fSlashCollider);
	fSlashEntity->addComponent(fSlashCollider);
	fSlashMovement = new Movement(fSlashEntity, 500 * cos(angle * M_PI / 180), 500 * sin(angle * M_PI / 180));//idek velX ir velY ir tada checkink collisionus
	fSlashEntity->addComponent(fSlashMovement);
	graphics->addToDraw(fSlashEntity);
	flyingSlashing = true;
	fSlashDir = playerAnimator->direction;
}

void PlayerAbilities::slashAttack() {
	slashEntity = new Entity();
	int width, height;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN || playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width / 2 - width / 2, playerTransform->globalPosY - height * 2 / 3);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width / 2 - width / 2, playerTransform->globalPosY + playerTransform->height);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::LEFT) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX - width, playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3);
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::RIGHT) {
		slashTransform = new Transform(slashEntity, width, height, playerTransform->globalPosX + playerTransform->width, playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3);
	}
	slashEntity->addComponent(slashTransform);
	SDL_Rect* src = new SDL_Rect();
	src->h = height;
	src->w = width;
	src->x = 0;
	src->y = playerAnimator->direction * src->h;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		src->y = playerAnimator->direction * 90 + 90 - src->h;
	}
	Drawable* drawable = new Drawable(slashEntity, slashAttackIMG, "slashAttack", Globals::PLAYER, src);
	slashEntity->addComponent(drawable);

	slashAnimator = new Animator(slashEntity);
	int slashSpeed = 30;
	// playerAnimator->direction * 4 + 0 + 1 + 2 + 3
	int dir = playerAnimator->direction * 4;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		dir = 24;
	}
	Animator::Animation slashingAnim("slashing", { dir + 0, dir + 1, dir + 2, dir + 3 }, slashSpeed, 4);
	slashAnimator->addAnimation(slashingAnim);
	slashEntity->addComponent(slashAnimator);
	slashAnimator->playAnimation("slashing");

	slashCollider = new Collider(slashEntity, Collider::ColliderType::NORMAL);
	CollisionSystem::collidersInScene.push_back(slashCollider);
	slashEntity->addComponent(slashCollider);

	slashing = true;
	graphics->addToDraw(slashEntity);
}

void PlayerAbilities::dashMove(int localPosX, int localPosY) {
	if (dashing) return;

	if (knocked) {
		knocked = false;
		PlayerMovement* pm = (PlayerMovement*)player->findComponent(ComponentType::MOVEMENT);
		pm->velX = 0;
		pm->velY = 0;
		pm->movementEnabled = true;
		playerAnimator->enabled = true;
		Collider* collider = (Collider*)player->findComponent(ComponentType::COLLIDER);
		collider->offset.x = 0;
		collider->offset.y = 0;
	}
	dashEntity = new Entity();
	int dashWidth, dashHeight;
	SDL_QueryTexture(dashIMG, NULL, NULL, &dashWidth, &dashHeight);
	dashHeight = 50;
	float dx = localPosX - (playerTransform->globalPosX + playerTransform->width / 2 - CameraSystem::posX);
	float dy = localPosY - (playerTransform->globalPosY + playerTransform->height / 2 - CameraSystem::posY);
	float angle = atan2(dy, dx) * 180 / M_PI;
	dashTransform = new Transform(dashEntity, sqrt(dx*dx + dy * dy), dashHeight, playerTransform->globalPosX + playerTransform->width / 2 + 10 * cos(angle * M_PI / 180), playerTransform->globalPosY + 10 * sin(angle * M_PI / 180));
	SDL_Point center;
	center.y = dashHeight / 2;
	center.x = 0;
	dashTransform->rotate(angle, &center);
	dashEntity->addComponent(dashTransform);
	SDL_Rect* src = new SDL_Rect();
	src->h = dashHeight;
	src->w = dashWidth;
	src->x = 0;
	src->y = 0;
	Drawable* dashDrawable = new Drawable(dashEntity, dashIMG, "dash", Globals::Layers::PLAYER, src);
	dashEntity->addComponent(dashDrawable);
	dashAnimator = new Animator(dashEntity);
	int dashSpeed = 50;
	Animator::Animation dashingAnim("dashing", { 0, 1, 2 }, dashSpeed, false);
	dashAnimator->addAnimation(dashingAnim);
	Animator::Animation notdashingAnim("notDashing", { 3, 3 }, dashSpeed, false);
	dashAnimator->addAnimation(notdashingAnim);
	dashEntity->addComponent(dashAnimator);
	SDL_Rect testCol;
	testCol.x = localPosX - Globals::TILE_SIZE / 2;
	testCol.y = localPosY - Globals::TILE_SIZE / 2;
	testCol.h = Globals::TILE_SIZE;
	testCol.w = Globals::TILE_SIZE;
	if (CollisionSystem::isCollidingWithEnv(testCol) || CollisionSystem::isCollidingWithObjects(testCol, { "" })) {
		dashAnimator->playAnimation("notDashing");
	}
	else {
		dashAnimator->playAnimation("dashing");
		// Some original mathematical shenanigans to calculate whether the dash collides with the soldier
		double dxSpeed = dx / Globals::TILE_SIZE;
		double dySpeed = dy / Globals::TILE_SIZE;
		for (double i = playerTransform->globalPosX, j = playerTransform->globalPosY; i != playerTransform->globalPosX + dx && j != playerTransform->globalPosY + dy; i += dxSpeed, j += dySpeed) {
			testCol.x = (i - playerTransform->width / 2) - CameraSystem::posX;
			testCol.y = (j - playerTransform->height / 2) - CameraSystem::posY;
			testCol.h = playerTransform->height;
			testCol.w = playerTransform->width;
			Collider* collision = CollisionSystem::isCollidingWithObjects(testCol, { "" });
			if (collision) {
				Drawable* drw = (Drawable*)collision->owner->findComponent(ComponentType::DRAWABLE);
				if (drw->ID == "soldier1" || drw->ID == "oldman" || drw->ID == "soldier2") {
					AIComponent* ai = (AIComponent*)collision->owner->findComponent(ComponentType::AI);
					if (drw->ID != "oldman") {
						ai->knockBack(20, 100, playerAnimator->direction, "dash");
					}
					else {
						if (!UIDesignSystem::isHealthShowing(collision->owner)) {
							UIDesignSystem::showHealth(collision->owner);
						}
					}
					Stats* enemyStats = (Stats*)collision->owner->findComponent(ComponentType::STATS);
					PlayerStats* playerStats = (PlayerStats*)player->findComponent(ComponentType::STATS);
					enemyStats->curHealth -= playerStats->dashAttackDmg;
					break; // Change if want multiple knocks have a temp value recording which was the knock to
				}
			}
		}
		playerTransform->globalPosX += dx;
		playerTransform->globalPosY += dy;
		Collider* pc = (Collider*)player->findComponent(ComponentType::COLLIDER);
		pc->colBox.x = (int)(playerTransform->globalPosX - CameraSystem::posX) + pc->offset.x;
		pc->colBox.y = (int)(playerTransform->globalPosY - CameraSystem::posY) + pc->offset.y;

		dashStart = SDL_GetTicks();
		if (CameraSystem::isCameraFollowing())
			CameraSystem::detachCamera();
	}

	graphics->addToDraw(dashEntity);
	dashing = true;

}

void PlayerAbilities::update(float deltaTime) {
	if (slashing) {
		slashUpdates(deltaTime);
	}
	if (dashing) {
		dashUpdates(deltaTime);
	}
	if (flyingSlashing) {
		fSlashUpdates(deltaTime);
	}
	if (playerStats->curHealth <= 0 && isAlive) {
		UIDesignSystem::createBloodshot(player);
		graphics->removeFromDraw(player);
		isAlive = false;
		if (fSlashEntity) {
			CollisionSystem::removeCollider(fSlashCollider);
			graphics->removeFromDraw(fSlashEntity);
			flyingSlashing = false;
			delete fSlashEntity;
			fSlashEntity = NULL;
		}

	}
	if (isKnocked()) {
		PlayerMovement* movement = (PlayerMovement*)player->findComponent(ComponentType::MOVEMENT);
		Collider* collider = (Collider*)player->findComponent(ComponentType::COLLIDER);
		Collider* didCollide = CollisionSystem::isCollidingWithObjects(collider, { knockedByAttackName });
		if (!didCollide) {
			didCollide = CollisionSystem::isCollidingWithEnv(collider);
		}
		if (didCollide) {
			knocked = false;
			movement->velX = 0;
			movement->velY = 0;
			movement->movementEnabled = true;
			playerAnimator->enabled = true;
			collider->offset.x = 0;
			collider->offset.y = 0;
		}
		if (knockDir == Animator::LookDirection::LEFT) {
			if (playerTransform->globalPosX <= result) {
				knocked = false;
				movement->velX = 0;
			}
		}
		if (knockDir == Animator::LookDirection::RIGHT) {
			if (playerTransform->globalPosX >= result) {
				knocked = false;
				movement->velX = 0;
			}
		}
		if (knockDir == Animator::LookDirection::UP) {
			if (playerTransform->globalPosY <= result) {
				knocked = false;
				movement->velY = 0;
			}
		}
		if (knockDir == Animator::LookDirection::DOWN) {
			if (playerTransform->globalPosY >= result) {
				knocked = false;
				movement->velY = 0;
			}
		}
		if (!knocked) {
			movement->movementEnabled = true;
			playerAnimator->enabled = true;
			collider->offset.x = 0;
			collider->offset.y = 0;
		}

	}
}

void PlayerAbilities::dashUpdates(float deltaTime) {
	dashEntity->update(deltaTime);
	if (dashStart == 0) {
		if (!dashAnimator->isAnimating()) {
			graphics->removeFromDraw(dashEntity);
			dashing = false;
			delete dashEntity;
			dashEntity = NULL;
		}
	}
	if (dashStart != 0 && SDL_GetTicks() - dashStart > 200) { //TODO figure out how to use players collider to do the attack's collision
		graphics->removeFromDraw(dashEntity);
		dashing = false;
		delete dashEntity;
		dashStart = 0;
		dashEntity = NULL;
		if (CameraSystem::allowedToMove)
			CameraSystem::moveAndFollow(playerTransform->globalPosX - Globals::SCREEN_WIDTH / 2, playerTransform->globalPosY - Globals::SCREEN_HEIGHT / 2, &playerTransform->globalPosX, &playerTransform->globalPosY, 1200);
	}

}

void PlayerAbilities::fSlashUpdates(float deltaTime) {
	fSlashEntity->update(deltaTime);
	if (!fSlashAnimator->isAnimating()) {
		fSlashAnimator->playAnimation("fSlashing", true);
	}


	Collider* collision = CollisionSystem::isCollidingWithObjects(fSlashCollider, { "player" });
	if (CollisionSystem::isCollidingWithEnv(fSlashCollider) || collision) {
		if (collision) {
			Drawable* colDrw = (Drawable*)collision->owner->findComponent(ComponentType::DRAWABLE);
			if (colDrw->ID == "soldier1" || colDrw->ID == "soldier2" || colDrw->ID == "oldman") {
				AIComponent* ai = (AIComponent*)collision->owner->findComponent(ComponentType::AI);
				Stats* enemyStats = (Stats*)collision->owner->findComponent(ComponentType::STATS);
				PlayerStats* playerStats = (PlayerStats*)player->findComponent(ComponentType::STATS);
				enemyStats->curHealth -= playerStats->fSlashAttackDmg;
				Drawable* fslashDrawable = (Drawable*)fSlashEntity->findComponent(ComponentType::DRAWABLE);
				ai->knockBack(200, 500, fSlashDir, fslashDrawable->ID);
			}
			if (colDrw->ID == "tree") {
				Animator* anim = (Animator*)collision->owner->findComponent(ComponentType::ANIMATOR);
				Stats* treeStats = (Stats*)collision->owner->findComponent(ComponentType::STATS);
				treeStats->curHealth -= 50;
				if (treeStats->curHealth == 0) {
					anim->playAnimation("treeHit");
					Collider* col = (Collider*)collision->owner->findComponent(ComponentType::COLLIDER);
					col->enabled = false;
					CollisionSystem::removeCollider(col);
					collision->owner->active = false;
				}
				else {
					anim->playAnimation("treeHit2");
					collision->owner->active = false;
				}
			}
		}
		CollisionSystem::removeCollider(fSlashCollider);
		graphics->removeFromDraw(fSlashEntity);
		flyingSlashing = false;
		delete fSlashEntity;
		fSlashEntity = NULL;
	}
}

void PlayerAbilities::knockBack(int dist, int speed, Animator::LookDirection dir, std::string attackName) {
	knockedByAttackName = attackName;
	knocked = true;
	knockDir = dir;
	playerAnimator->stopAnimation();
	playerAnimator->enabled = false;
	PlayerMovement* pm = (PlayerMovement*)player->findComponent(ComponentType::MOVEMENT);
	pm->movementEnabled = false;
	Collider* pc = (Collider*)player->findComponent(ComponentType::COLLIDER);

	int colBoxChangeVal = 10; // Changing the collision box a bit to test whether or not it will collide so to not get "stuck"
	if (dir == Animator::LookDirection::LEFT) {
		result = playerTransform->globalPosX - dist;
		pm->velX -= speed;
		pc->offset.x -= colBoxChangeVal;
	}
	if (dir == Animator::LookDirection::RIGHT) {
		result = playerTransform->globalPosX + dist;
		pm->velX += speed;
		pc->offset.x += colBoxChangeVal;
	}
	if (dir == Animator::LookDirection::UP) {
		result = playerTransform->globalPosY - dist;
		pm->velY -= speed;
		pc->offset.y -= colBoxChangeVal;
	}
	if (dir == Animator::LookDirection::DOWN) {
		result = playerTransform->globalPosY + dist;
		pm->velY += speed;
		pc->offset.y += colBoxChangeVal;
	}
}

void PlayerAbilities::slashUpdates(float deltaTime) {
	slashEntity->update(deltaTime);

	int width, height;
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN || playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		width = 90;
		height = 50;
	}
	else {
		width = 50;
		height = 90;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::UP) {
		slashTransform->globalPosX = playerTransform->globalPosX + playerTransform->width / 2 - width / 2;
		slashTransform->globalPosY = playerTransform->globalPosY - height * 2 / 3;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::DOWN) {
		slashTransform->globalPosX = playerTransform->globalPosX + playerTransform->width / 2 - width / 2;
		slashTransform->globalPosY = playerTransform->globalPosY + playerTransform->height;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::LEFT) {
		slashTransform->globalPosX = playerTransform->globalPosX - width;
		slashTransform->globalPosY = playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3;
	}
	if (playerAnimator->direction == PlayerAnimator::LookDirection::RIGHT) {
		slashTransform->globalPosX = playerTransform->globalPosX + playerTransform->width;
		slashTransform->globalPosY = playerTransform->globalPosY + playerTransform->height / 2 - width * 2 / 3;
	}

	Collider* slashCollision = CollisionSystem::isCollidingWithObjects(slashCollider, {});
	if (slashCollider->enabled && slashCollision) {
		Drawable* draw = (Drawable*)slashCollision->owner->findComponent(ComponentType::DRAWABLE);
		Transform* trans = (Transform*)slashCollision->owner->findComponent(ComponentType::TRANSFORM);
		if (slashCollision->owner->hasComponent(ComponentType::AI)) {
			AIComponent* ai = (AIComponent*)slashCollision->owner->findComponent(ComponentType::AI);
			if ((draw->ID == "soldier2" || draw->ID == "soldier1" || draw->ID == "oldman") && !ai->isKnocked()) { // can use substrings to know type (soldier)
				slashCollider->enabled = false;
				Stats* enemyStats = (Stats*)slashCollision->owner->findComponent(ComponentType::STATS);
				PlayerStats* playerStats = (PlayerStats*)player->findComponent(ComponentType::STATS);
				enemyStats->curHealth -= playerStats->mainAttackDmg;
				Drawable* slashDrawable = (Drawable*)slashEntity->findComponent(ComponentType::DRAWABLE);
				if (draw->ID != "oldman") {
					ai->knockBack(100, 500, playerAnimator->direction, slashDrawable->ID);
				}
				else {
					if (!UIDesignSystem::isHealthShowing(slashCollision->owner)) {
						UIDesignSystem::showHealth(slashCollision->owner);
					}
				}
			}
		}

	}

	if (!slashAnimator->isAnimating()) {
		slashCollider->enabled = true;
		CollisionSystem::removeCollider(slashCollider);
		slashing = false;
		graphics->removeFromDraw(slashEntity);
		delete slashEntity;
		slashEntity = NULL;
	}
}


