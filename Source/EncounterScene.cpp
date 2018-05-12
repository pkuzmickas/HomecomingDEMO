#include "EncounterScene.h"

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, ASSET_DIR LEVEL_DIR "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());
	CameraSystem::setUp(MapSystem::getWidth(), MapSystem::getHeight()); // need to set up the camera right when the map is loaded
	DialogueSystem::loadDialogues(ASSET_DIR DIALOGUE_DIR "FirstEncounter.xml");

	setup();

}

void EncounterScene::setup() {

	// Spawning npcs
	oldman = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "oldman.png");
	soldier = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "soldier.png");
	zoro = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "zoro.png");
	Entity* oldmanEntity = NPCSystem::createBoss(270, 700, 48, 48, Globals::Layers::PLAYER, oldman, "oldman", renderer, graphics);
	Entity* soldierEntity = NPCSystem::createSoldier(200, 700, 48, 48, Globals::Layers::PLAYER, zoro, "soldier1", renderer, graphics, 50, 400); //zoro
	Entity* soldier2Entity = NPCSystem::createSoldier(340, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier2", renderer, graphics, 50, 200);
	oldmanAI = (AIBoss*)oldmanEntity->findComponent(ComponentType::AI);
	soldierAI = (AISoldier*)soldierEntity->findComponent(ComponentType::AI);
	soldier2AI = (AISoldier*)soldier2Entity->findComponent(ComponentType::AI);
	graphics->addToDraw(oldmanEntity);
	graphics->addToDraw(soldierEntity);
	graphics->addToDraw(soldier2Entity);
	entities.push_back(oldmanEntity);
	entities.push_back(soldierEntity);
	entities.push_back(soldier2Entity);

	createPlayer(1800, 700, Animator::LookDirection::LEFT);
	PlayerSystem::disableMovement();

	// Spawning trees
	tree = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "tree.png");
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < MapSystem::getWidth() / 103 + 1; i++) {

			SDL_Rect colOffset; //0, 100, 103, 25
			SDL_Rect* colPtr = &colOffset;
			if (j == 0) {
				colOffset.x = 0;
				colOffset.y = -40;
				colOffset.w = 0;
				colOffset.h = 0;
			}
			else {
				colOffset.x = 0;
				colOffset.y = 100;
				colOffset.w = 103;
				colOffset.h = 40;
			}
			SDL_Rect* srcRect = new SDL_Rect();
			srcRect->h = 156;
			srcRect->w = 122;
			srcRect->x = 0;
			srcRect->y = 0;
			//tree no leaves
			Entity* e = SceneDesignSystem::createTree(i * srcRect->w, 550 + j * srcRect->h, (Globals::Layers)(Globals::Layers::BACKGROUND2 + j), tree, colPtr, srcRect);
			Stats* treeStats = (Stats*)e->findComponent(ComponentType::STATS);
			treeStats->totalHealth = 50;
			treeStats->curHealth = 50;
			graphics->addToDraw(e);
			entities.push_back(e);
			if (i * 122 < MapSystem::getWidth()) {
				if (j == 0) {
					colPtr = NULL;
				}
				else {
					colOffset.x = 0;
					colOffset.y = 50;
					colOffset.w = 0;
					colOffset.h = 0;

				}
				SDL_Rect* srcRect = new SDL_Rect();
				srcRect->h = 156;
				srcRect->w = 122;
				srcRect->x = 0;
				srcRect->y = srcRect->h * 3;
				Entity* e2 = SceneDesignSystem::createTree(i * 122, 475 + j * 305, (Globals::Layers)(Globals::Layers::BACKGROUND1 + 3 * j), tree, colPtr, srcRect);
				graphics->addToDraw(e2);
				entities.push_back(e2);
			}
		}
	}


	CameraSystem::posY = MapSystem::getHeight() - Globals::SCREEN_HEIGHT;
	Entity* blackBox1 = SceneDesignSystem::createRect(CameraSystem::posX, CameraSystem::posY, Globals::SCREEN_WIDTH + 10, Globals::SCREEN_HEIGHT / 2, Globals::Layers::UI, true); // -85 is a good number
	Entity* blackBox2 = SceneDesignSystem::createRect(CameraSystem::posX, CameraSystem::posY + Globals::SCREEN_HEIGHT / 2, Globals::SCREEN_WIDTH + 10, Globals::SCREEN_HEIGHT / 2, Globals::Layers::UI, true);
	blackBox1T = (Transform*)blackBox1->findComponent(ComponentType::TRANSFORM);
	blackBox2T = (Transform*)blackBox2->findComponent(ComponentType::TRANSFORM);
	graphics->addToDraw(blackBox1);
	graphics->addToDraw(blackBox2);
	entities.push_back(blackBox1);
	entities.push_back(blackBox2);

	//wait(2, "intro text");

	//for testing
	
	graphics->removeFromDraw(player);
	curAction = "restart";
	Transform* solt = (Transform*)(oldmanEntity->findComponent(ComponentType::TRANSFORM));
	solt->globalPosX = 1180;
	/*Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
	CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
	PlayerSystem::enableMovement();
	Transform* solt = (Transform*)(soldier2Entity->findComponent(ComponentType::TRANSFORM));
	solt->globalPosX = 1500;
	//soldier2AI->attack(player);
	//soldierAI->attack(player);
	UIDesignSystem::showPlayerHealth(player);
	*/
}

void EncounterScene::preFightScenario(float deltaTime) {
	if (curAction == "intro text") {
		int fontSize = 50;
		if (textProgress > 1) {
			SDL_DestroyTexture(textTexture);
			string text = introText.substr(0, textProgress);
			TTF_Font* font = TTF_OpenFont(ASSET_DIR FONT_PATH, fontSize);
			SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), { 255, 255, 255, 0xFF });
			SDL_Texture* textTexturee = SDL_CreateTextureFromSurface(renderer, textSurface);
			Drawable* drawable = (Drawable*)textEntity->findComponent(ComponentType::DRAWABLE);
			drawable->image = textTexturee;
			textTexture = drawable->image;
			Transform* transform = (Transform*)textEntity->findComponent(ComponentType::TRANSFORM);
			transform->width = textSurface->w;
			transform->height = textSurface->h;
			SDL_FreeSurface(textSurface);
			TTF_CloseFont(font);
			textProgress++;
		}
		else {
			string text = introText.substr(0, textProgress);
			int totalTextWidth, textHeight;
			SceneDesignSystem::checkFontSizeOnText(introText, fontSize, &totalTextWidth, &textHeight);
			textEntity = SceneDesignSystem::createText(text, CameraSystem::posX + Globals::SCREEN_WIDTH / 2 - totalTextWidth / 2, CameraSystem::posY + Globals::SCREEN_HEIGHT / 2 - textHeight / 2, fontSize, { 255,255,255,0xFF }, Globals::Layers::UI, renderer);
			textTexture = ((Drawable*)textEntity->findComponent(ComponentType::DRAWABLE))->image;
			graphics->addToDraw(textEntity);
			textProgress++;
		}


		if (textProgress > introText.length()) {
			wait(2, "delete text");
		}
		else {
			wait(0.2f, "intro text");
		}
	}
	if (curAction == "delete text") {
		graphics->removeFromDraw(textEntity);
		SceneDesignSystem::cleanupText(textEntity);
		wait(2, "open shades");
	}
	if (curAction == "open shades") {
		blackBox1T->height = 0;
		blackBox2T->height = 0;
		wait(1, "find action");
	}
	if (curAction == "find action") {
		Transform* oldmanT = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
		CameraSystem::moveCamera(CameraSystem::posX, oldmanT->globalPosY - Globals::SCREEN_HEIGHT / 2, 1000);
		curAction = "start walking";
	}
	if (curAction != "camera moving 1") {
		blackBox1T->globalPosX = CameraSystem::posX;
		blackBox2T->globalPosX = CameraSystem::posX;
	}
	if (curAction == "start walking") {
		Transform* oldmanT = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
		soldier2AI->walkTo(1250, 700, 80);
		oldmanAI->walkTo(1180, 700, 80);
		soldierAI->walkTo(1110, 700, 80);
		curAction = "npcs walking";
	}
	if (curAction == "npcs walking") {
		if (!CameraSystem::isCameraMoving() && !CameraSystem::isCameraFollowing()) {
			Transform* oldmanT = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
			CameraSystem::follow(&oldmanT->globalPosX, &oldmanT->globalPosY);
			curAction = "shades close";
		}
	}
	if (curAction == "shades close") {
		if (blackBox1T->height == 0) {
			blackBox1T->height = Globals::SCREEN_HEIGHT / 2 - 85;
			blackBox2T->height = Globals::SCREEN_HEIGHT / 2 + 85;
			blackBox1T->globalPosY = CameraSystem::posY - blackBox1T->height;
			blackBox2T->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT;
		}
		if (CameraSystem::posY - blackBox1T->globalPosY > 0) {
			blackBox1T->globalPosY += 300 * deltaTime;
			blackBox2T->globalPosY -= 300 * deltaTime;
		}
		else {
			blackBox1T->globalPosY = CameraSystem::posY;
			blackBox2T->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT / 2 + 85;
		}
		if (!soldierAI->isWalking()) {
			wait(2, "camera switch 1");
		}
	}
	if (curAction == "camera switch 1") {
		CameraSystem::detachCamera();
		Transform* soldierTransform = (Transform*)(soldierAI->owner->findComponent(ComponentType::TRANSFORM));
		CameraSystem::moveCamera(soldierTransform->globalPosX - 100, CameraSystem::posY, 100);

		curAction = "camera moving 1";
	}
	if (curAction == "camera moving 1") {
		if (blackBox1T->height > 0) {
			blackBox1T->globalPosX = CameraSystem::posX;
			blackBox2T->globalPosX = CameraSystem::posX;
			blackBox1T->globalPosY -= 100 * deltaTime;
			blackBox2T->globalPosY += 100 * deltaTime;
		}
		if (!CameraSystem::isCameraMoving()) {
			blackBox1T->height = 0;
			blackBox2T->height = 0;
			DialogueSystem::openDialogueBox("pre-fight");
			curAction = "dialogue";
		}
	}
	if (curAction == "dialogue") {
		if (!DialogueSystem::isOpen()) {
			curAction = "move camera to player";
		}
	}
	if (curAction == "move camera to player") {
		Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
		CameraSystem::moveCamera(playerTransform->globalPosX - Globals::SCREEN_WIDTH / 2, CameraSystem::posY, 400);
		curAction = "camera moving 2";
	}
	if (curAction == "camera moving 2") {
		if (!CameraSystem::isCameraMoving()) {
			Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
			CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
			PlayerSystem::enableMovement();
			curAction = "fighting soldiers";
			soldier2AI->attack(player);
			UIDesignSystem::showPlayerHealth(player);
			boundary1 = SceneDesignSystem::createBoundary(CameraSystem::posX, CameraSystem::posY, Globals::TILE_SIZE, Globals::SCREEN_HEIGHT);
			boundary2 = SceneDesignSystem::createBoundary(CameraSystem::posX + Globals::SCREEN_WIDTH - Globals::TILE_SIZE, CameraSystem::posY, Globals::TILE_SIZE, Globals::SCREEN_HEIGHT);
			CameraSystem::detachCamera();
			graphics->addToDraw(boundary1);
			graphics->addToDraw(boundary2);
		}
	}
	if (curAction == "fighting soldiers") {
		CameraSystem::allowedToMove = false;
		if (soldier2AI->state == AISoldier::DEAD) {
			graphics->removeFromDraw(boundary1);
			graphics->removeFromDraw(boundary2);
			Collider* c1 = (Collider*)boundary1->findComponent(ComponentType::COLLIDER);
			Collider* c2 = (Collider*)boundary2->findComponent(ComponentType::COLLIDER);
			CollisionSystem::removeCollider(c1);
			CollisionSystem::removeCollider(c2);
			Transform* ot = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
			CameraSystem::moveCamera(ot->globalPosX - 100, ot->globalPosY - Globals::SCREEN_HEIGHT / 2, 200);
			PlayerSystem::disableMovement();
			
			curAction = "camera moving 3";
		}
	}
	if (curAction == "camera moving 3") {
		if (!CameraSystem::isCameraMoving()) {
			curAction = "pre talk";
			PlayerAbilities* pa = (PlayerAbilities*)player->findComponent(ComponentType::ABILITIES);
			Transform* ot = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
			pa->dashMove(ot->globalPosX + Globals::SCREEN_WIDTH - 200 - CameraSystem::posX, ot->globalPosY - CameraSystem::posY + ot->height / 2);
			Drawable* drw = (Drawable*)player->findComponent(ComponentType::DRAWABLE);
			drw->srcRect->y = PlayerAnimator::LookDirection::LEFT * drw->srcRect->h;
			PlayerAnimator* panim = (PlayerAnimator*)player->findComponent(ComponentType::ANIMATOR);
			panim->direction = PlayerAnimator::LookDirection::LEFT;
			panim->update(deltaTime);
			wait(1, "open dialogue fight1");	
		}
	}
	if (curAction == "open dialogue fight1") {
		DialogueSystem::openDialogueBox("fight1");
		curAction = "pre talk";
	}
	if (curAction == "pre talk") {
		if (!DialogueSystem::isOpen()) {
			CameraSystem::allowedToMove = true;
			Transform* pt = (Transform*)player->findComponent(ComponentType::TRANSFORM);
			CameraSystem::moveAndFollow(pt->globalPosX - Globals::SCREEN_WIDTH / 2, pt->globalPosY - Globals::SCREEN_HEIGHT / 2, &pt->globalPosX, &pt->globalPosY, 400);
			PlayerSystem::enableMovement();
			oldmanAI->slashAttack(pt->globalPosX - CameraSystem::posX, pt->globalPosY - CameraSystem::posY - 100);
			oldmanAI->slashAttack(pt->globalPosX - CameraSystem::posX, pt->globalPosY - CameraSystem::posY);
			oldmanAI->slashAttack(pt->globalPosX - CameraSystem::posX, pt->globalPosY - CameraSystem::posY + 150);
			PlayerAbilities* pa = (PlayerAbilities*)player->findComponent(ComponentType::ABILITIES);
			pa->dashMove(pt->globalPosX + 200 - CameraSystem::posX, pt->globalPosY - CameraSystem::posY + pt->height/2);

			curAction = "oldman slashing";
		}
	}
	if (curAction == "oldman slashing") {
		if (oldmanAI->slashesInUse.size() == 0) {
			CameraSystem::detachCamera();
			Transform* ot = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
			CameraSystem::moveCamera(ot->globalPosX - 100, CameraSystem::posY, 400);
			PlayerSystem::disableMovement();
			curAction = "camera moving 4";
		}
	}
	if (curAction == "camera moving 4") {
		if (!CameraSystem::isCameraMoving()) {
			DialogueSystem::openDialogueBox("fight2");
			curAction = "open dialogue fight2";
		}
	}
	if (curAction == "open dialogue fight2") {
		if (!DialogueSystem::isOpen()) {
			soldierAI->attack(player);
			Transform* ot = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
			CameraSystem::moveCamera(ot->globalPosX + 100, ot->globalPosY - Globals::SCREEN_HEIGHT / 2, 400);
			PlayerSystem::enableMovement();
			CameraSystem::allowedToMove = false;
			curAction = "camera moving 5";
		}
	}
	if (curAction == "camera moving 5") {
		if (!CameraSystem::isCameraMoving()) {
			CameraSystem::detachCamera();
			graphics->addToDraw(boundary1);
			graphics->addToDraw(boundary2);
			Collider* c1 = (Collider*)boundary1->findComponent(ComponentType::COLLIDER);
			Collider* c2 = (Collider*)boundary2->findComponent(ComponentType::COLLIDER);
			CollisionSystem::collidersInScene.push_back(c1);
			CollisionSystem::collidersInScene.push_back(c2);
			Transform* b1t = (Transform*)boundary1->findComponent(ComponentType::TRANSFORM);
			Transform* b2t = (Transform*)boundary2->findComponent(ComponentType::TRANSFORM);
			b1t->globalPosX = CameraSystem::posX;
			b2t->globalPosX = CameraSystem::posX + Globals::SCREEN_WIDTH - b2t->width;
			curAction = "fighting soldiers 2";
		}
	}
	if (curAction == "fighting soldiers 2") {
		if (soldierAI->state == AIComponent::State::DEAD) {
			PlayerAbilities* pa = (PlayerAbilities*)player->findComponent(ComponentType::ABILITIES);
			Transform* ot = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
			pa->dashMove(ot->globalPosX + Globals::SCREEN_WIDTH - 200 - CameraSystem::posX, ot->globalPosY - CameraSystem::posY + ot->height / 2);
			Drawable* drw = (Drawable*)player->findComponent(ComponentType::DRAWABLE);
			drw->srcRect->y = PlayerAnimator::LookDirection::LEFT * drw->srcRect->h;
			PlayerAnimator* panim = (PlayerAnimator*)player->findComponent(ComponentType::ANIMATOR);
			panim->direction = PlayerAnimator::LookDirection::LEFT;
			panim->update(deltaTime);
			graphics->removeFromDraw(boundary1);
			graphics->removeFromDraw(boundary2);
			Collider* c1 = (Collider*)boundary1->findComponent(ComponentType::COLLIDER);
			Collider* c2 = (Collider*)boundary2->findComponent(ComponentType::COLLIDER);
			CollisionSystem::removeCollider(c1);
			CollisionSystem::removeCollider(c2);
			CameraSystem::moveCamera(ot->globalPosX - 100, ot->globalPosY - Globals::SCREEN_HEIGHT / 2, 200);
			PlayerSystem::disableMovement();
			curAction = "camera moving 6";
		}
	}
	if (curAction == "camera moving 6") {
		if (!CameraSystem::isCameraMoving()) {
			wait(1, "open dialogue fight3");
		}
	}
	if (curAction == "open dialogue fight3") {
		DialogueSystem::openDialogueBox("fight3");
		curAction = "final dialogue";
	}
	if (curAction == "final dialogue") {
		if (!DialogueSystem::isOpen()) {
			
			Transform* pt = (Transform*)player->findComponent(ComponentType::TRANSFORM);
			CameraSystem::moveAndFollow(pt->globalPosX - Globals::SCREEN_WIDTH / 2, pt->globalPosY - Globals::SCREEN_HEIGHT / 2, &pt->globalPosX, &pt->globalPosY, 400);
			PlayerSystem::enableMovement();
			CameraSystem::allowedToMove = true;

			oldmanAI->attack(player);
			curAction = "";
		}
	}
}

void EncounterScene::update(float deltaTime) {
	
	Scene::update(deltaTime);

	for (int i = 0; i < (int)entities.size(); i++) {
		Entity* ent = entities[i];

		if (!ent->active && ent->hasComponent(ComponentType::ANIMATOR)) {
			Animator* anim = (Animator*)ent->findComponent(ComponentType::ANIMATOR);
			Drawable* drw = (Drawable*)ent->findComponent(ComponentType::DRAWABLE);

			if ((!anim->isAnimating() && drw->ID == "tree") || drw->ID != "tree") {
				if (drw->ID == "tree") {
					Stats* treeStats = (Stats*)ent->findComponent(ComponentType::STATS);
					if (treeStats->curHealth == 50) {
						ent->active = true;
						affectedEntities.push_back(ent);
						continue;
					}
				}
				graphics->removeFromDraw(ent);
				iter_swap(entities.begin() + i, entities.begin() + entities.size() - 1);
				entities.pop_back();
				//delete ent;
				removedEntities.push_back(ent);
				
			}
			else {
				ent->update(deltaTime);
			}
		}
		else if (!ent->active) {
			graphics->removeFromDraw(ent);
			iter_swap(entities.begin() + i, entities.begin() + entities.size() - 1);
			entities.pop_back();
			//delete ent;
			removedEntities.push_back(ent);
		}
		else {
			ent->update(deltaTime);
		}
		if (boundary1) {
			boundary1->update(deltaTime);
			boundary2->update(deltaTime);
		}

		
	}
	
	preFightScenario(deltaTime);
	if (curAction == "restart") {
		curAction = "fighting soldiers";
		hideLoseScreen();
		PlayerSystem::resetPlayer();
		graphics->addToDraw(player);
		Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
		playerTransform->globalPosX = 1800;
		playerTransform->globalPosY = 700;
		//CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
		PlayerSystem::enableMovement();
		Transform* solt = (Transform*)(soldierAI->owner->findComponent(ComponentType::TRANSFORM));
		solt->globalPosX = 1110;
		solt->globalPosY = 700;
		Transform* solt2 = (Transform*)(soldier2AI->owner->findComponent(ComponentType::TRANSFORM));
		solt2->globalPosX = 1250;
		solt2->globalPosY = 700;
		Transform* ot = (Transform*)(oldmanAI->owner->findComponent(ComponentType::TRANSFORM));
		solt2->globalPosX = 1250;
		solt2->globalPosY = 700;
		//Transform* oldt = (Transform*)(oldmanAI->owner->findComponent(ComponentType::TRANSFORM));
		//solt->globalPosX = 1180;

		//soldierAI->attack(player);
		UIDesignSystem::showPlayerHealth(player);
		//do not delete on kill, remove from draw and remove colliders

		soldier2AI->state = AIComponent::State::NORMAL;
		soldierAI->state = AIComponent::State::NORMAL;
		soldier2AI->subState = AISoldier::subStates::NONE;
		soldierAI->subState = AISoldier::subStates::NONE;
		oldmanAI->state = AIComponent::State::NORMAL;
		oldmanAI->subState = AIBoss::subStates::NONE;
		Stats* stats = (Stats*)soldier2AI->owner->findComponent(ComponentType::STATS);
		stats->curHealth = stats->totalHealth;
		stats = (Stats*)soldierAI->owner->findComponent(ComponentType::STATS);
		stats->curHealth = stats->totalHealth;
		stats = (Stats*)oldmanAI->owner->findComponent(ComponentType::STATS);
		stats->curHealth = stats->totalHealth;
		UIDesignSystem::hideHealth(soldier2AI->owner);
		UIDesignSystem::hideHealth(soldierAI->owner);
		UIDesignSystem::hideHealth(oldmanAI->owner);

		for (auto ent : removedEntities) {
			graphics->addToDraw(ent);
			entities.push_back(ent);
			Collider* col = (Collider*)ent->findComponent(ComponentType::COLLIDER);
			if (!col->enabled) {
				col->enabled = true;
				CollisionSystem::collidersInScene.push_back(col);
				ent->active = true;
			}
			Drawable* drw = (Drawable*)ent->findComponent(ComponentType::DRAWABLE);
			if (drw->ID == "tree") {
				/*Animator* anim = (Animator*)ent->findComponent(ComponentType::ANIMATOR);
				anim->stopAnimation();*/
				//drw->srcRect = drw->startingSrcRect;
				*drw->srcRect = drw->startingSrcRect;
			}
			Stats* stats = (Stats*)ent->findComponent(ComponentType::STATS);
			stats->curHealth = stats->totalHealth;
		}
		removedEntities.clear();
		for (auto ent : affectedEntities) {
			Stats* stats = (Stats*)ent->findComponent(ComponentType::STATS);
			stats->curHealth = stats->totalHealth;
			Drawable* drw = (Drawable*)ent->findComponent(ComponentType::DRAWABLE);
			*drw->srcRect = drw->startingSrcRect;
		}
		affectedEntities.clear();
		soldier2AI->attack(player);
		CameraSystem::detachCamera();
		CameraSystem::posX = playerTransform->globalPosX - Globals::SCREEN_WIDTH / 2;
		CameraSystem::posY = playerTransform->globalPosY - Globals::SCREEN_HEIGHT / 2;
		if (!boundary1) {
			
			boundary1 = SceneDesignSystem::createBoundary(CameraSystem::posX, CameraSystem::posY, Globals::TILE_SIZE, Globals::SCREEN_HEIGHT);
			boundary2 = SceneDesignSystem::createBoundary(CameraSystem::posX + Globals::SCREEN_WIDTH - Globals::TILE_SIZE, CameraSystem::posY, Globals::TILE_SIZE, Globals::SCREEN_HEIGHT);
			graphics->addToDraw(boundary1);
			graphics->addToDraw(boundary2);
			
		}
	}

}

EncounterScene::~EncounterScene() {
	SDL_DestroyTexture(tree);
	SDL_DestroyTexture(oldman);
	SDL_DestroyTexture(soldier);
	SDL_DestroyTexture(zoro);
	if (boundary1) {
		delete boundary1;
		delete boundary2;
		boundary1 = NULL;
		boundary2 = NULL;
	}
	for (auto tree : entities) {
		delete tree;
	}
}


