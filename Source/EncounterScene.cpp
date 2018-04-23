#include "EncounterScene.h"

EncounterScene::EncounterScene(SDL_Renderer * renderer, Graphics * graphics) : Scene(renderer, graphics) {
	MapSystem::createMap(renderer, ASSET_DIR LEVEL_DIR "demoMap.tmx");
	graphics->addMap(*MapSystem::getMap());
	CameraSystem::setUp(MapSystem::getWidth(), MapSystem::getHeight()); // need to set up the camera right when the map is loaded
	DialogueSystem::loadDialogues(ASSET_DIR DIALOGUE_DIR "FirstEncounter.xml");

	setup();

}

void EncounterScene::setup() {

	createPlayer(1800, 700, Animator::LookDirection::LEFT);
	PlayerSystem::disableMovement();

	

	// Spawning npcs
	oldman = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "oldman.png");
	soldier = IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "soldier.png");
	Entity* oldmanEntity = NPCSystem::createNPC(270, 700, 48, 48, Globals::Layers::PLAYER, oldman, "oldman");
	Entity* soldierEntity = NPCSystem::createNPC(200, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier1");
	Entity* soldier2Entity = NPCSystem::createNPC(340, 700, 48, 48, Globals::Layers::PLAYER, soldier, "soldier2");
	oldmanAI = (AIComponent*)oldmanEntity->findComponent(ComponentType::AI);
	soldierAI = (AIComponent*)soldierEntity->findComponent(ComponentType::AI);
	soldier2AI = (AIComponent*)soldier2Entity->findComponent(ComponentType::AI);
	graphics->addToDraw(oldmanEntity);
	graphics->addToDraw(soldierEntity);
	graphics->addToDraw(soldier2Entity);
	entities.push_back(oldmanEntity);
	entities.push_back(soldierEntity);
	entities.push_back(soldier2Entity);
	


	// Spawning trees
	tree = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "tree.png");
	treeNoLeaves = IMG_LoadTexture(renderer, ASSET_DIR LEVEL_DESIGN_DIR "treeNoLeaves.png");
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < MapSystem::getWidth() / 103 + 1; i++) {
			Entity* e = SceneDesignSystem::createTree(i * 103, 550 + j * 150, (Globals::Layers)(Globals::Layers::BACKGROUND2 + j), treeNoLeaves);
			graphics->addToDraw(e);
			entities.push_back(e);
			if (i * 122 < MapSystem::getWidth()) {
				Entity* e2 = SceneDesignSystem::createTree(i * 122, 475 + j * 305, (Globals::Layers)(Globals::Layers::BACKGROUND1 + 3 * j), tree);
				graphics->addToDraw(e2);
				entities.push_back(e2);
			}
		}
	}

	
	CameraSystem::posY = MapSystem::getHeight() - Globals::SCREEN_HEIGHT;
	Entity* blackBox1 = SceneDesignSystem::createRect(CameraSystem::posX, CameraSystem::posY, Globals::SCREEN_WIDTH + 10, Globals::SCREEN_HEIGHT/2, Globals::Layers::UI, true); // -85 is a good number
	Entity* blackBox2 = SceneDesignSystem::createRect(CameraSystem::posX, CameraSystem::posY + Globals::SCREEN_HEIGHT / 2, Globals::SCREEN_WIDTH + 10, Globals::SCREEN_HEIGHT/2, Globals::Layers::UI, true);
	blackBox1T = (Transform*) blackBox1->findComponent(ComponentType::TRANSFORM);
	blackBox2T = (Transform*)blackBox2->findComponent(ComponentType::TRANSFORM);
	//graphics->addToDraw(blackBox1);
	//graphics->addToDraw(blackBox2);
	entities.push_back(blackBox1);
	entities.push_back(blackBox2);
	
	wait(2, "intro text");
	
	//for testing
	Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
	CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
	PlayerSystem::enableMovement();

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
		//curAction = "find action";
	}
	if (curAction == "find action") {
		Transform* oldmanT = (Transform*)oldmanAI->owner->findComponent(ComponentType::TRANSFORM);
		CameraSystem::moveCamera(CameraSystem::posX, oldmanT->globalPosY - Globals::SCREEN_HEIGHT/2, 1000);
		curAction = "start walking";
	}
	if (curAction!= "camera moving 1") {
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
		//DialogueSystem::closeDialogueBox();
		Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
		CameraSystem::moveCamera(playerTransform->globalPosX - Globals::SCREEN_WIDTH / 2, CameraSystem::posY, 400);
		curAction = "camera moving 2";
	}
	if (curAction == "camera moving 2") {
		if (!CameraSystem::isCameraMoving()) {
			Transform* playerTransform = (Transform*)(player->findComponent(ComponentType::TRANSFORM));
			CameraSystem::follow(&playerTransform->globalPosX, &playerTransform->globalPosY);
			PlayerSystem::enableMovement();
			curAction = "";
		}
	}
}

void EncounterScene::update(float deltaTime) {
	Scene::update(deltaTime);
	for (auto ent : entities) {
		ent->update(deltaTime);
	}
	
	//preFightScenario(deltaTime);
	
}

EncounterScene::~EncounterScene() {
	SDL_DestroyTexture(tree);
	SDL_DestroyTexture(treeNoLeaves);
	SDL_DestroyTexture(oldman);
	SDL_DestroyTexture(soldier);
	for (auto tree : entities) {
		delete tree;
	}
}


