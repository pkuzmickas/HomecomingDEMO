#include "Scene.h"

Scene::~Scene() {
	if(player) delete player;
	MapSystem::deleteMap();
	DialogueSystem::cleanup();
	UIDesignSystem::cleanup();
	SDL_DestroyTexture(diedIMG);
	SDL_DestroyTexture(startIMG);
	SDL_DestroyTexture(endIMG);
	SDL_DestroyTexture(infoIMG);
	if (loseBox) delete loseBox;
	if (loseText) delete loseText;
	if(endScreen) delete endScreen;
	if (controlsScreen) delete controlsScreen;
}

void Scene::update(float deltaTime) {
	if (player && !gameOver) {
		player->update(deltaTime);
		PlayerStats* ps = (PlayerStats*)player->findComponent(ComponentType::STATS);
		if (ps->curHealth <= 0) {
			gameOver = true;
			wait(1, "gameover");
		}
	}
	if (curAction == "waiting") {
		if (SDL_GetTicks() - waitTimePassed > waitSeconds * 1000) {
			waitSeconds = 0;
			waitTimePassed = 0;
			curAction = nextAction;
			nextAction = "";
		}
	}
	if (curAction == "gameover") {
		showLoseScreen();
		curAction = "";
	}
	DialogueSystem::update(deltaTime);
	
}

Scene::Scene(SDL_Renderer * renderer, Graphics * graphics) {
	this->renderer = renderer;
	this->graphics = graphics;
	DialogueSystem::setup(graphics, renderer);
	UIDesignSystem::setup(renderer, graphics);
	diedIMG = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "death.png");
	startIMG = IMG_LoadTexture(renderer, ASSET_DIR "titleScreen.png");
	endIMG = IMG_LoadTexture(renderer, ASSET_DIR "tbc.png");
	infoIMG = IMG_LoadTexture(renderer, ASSET_DIR "controls.png");
}

void Scene::createPlayer(int globalPosX, int globalPosY, PlayerAnimator::LookDirection lookDirection) {
	player = PlayerSystem::createPlayer(globalPosX, globalPosY, IMG_LoadTexture(renderer, ASSET_DIR CHARACTER_DIR "player.png"), renderer, graphics,lookDirection);
	graphics->addToDraw(player);
}

void Scene::wait(float waitSeconds, std::string nextAction) {
	this->waitSeconds = waitSeconds;
	this->nextAction = nextAction;
	waitTimePassed = (int)SDL_GetTicks();
	curAction = "waiting";
}

void Scene::showLoseScreen() {
	loseBox = SceneDesignSystem::createRect(CameraSystem::posX, CameraSystem::posY, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT,Globals::Layers::UI, true);
	graphics->addToDraw(loseBox);
	loseText = new Entity();
	int width, height;
	SDL_QueryTexture(diedIMG, NULL, NULL, &width, &height);
	Transform* t = new Transform(loseText, width, height, CameraSystem::posX + Globals::SCREEN_WIDTH / 2 - width / 2, CameraSystem::posY + Globals::SCREEN_HEIGHT / 2 - height / 2);
	loseText->addComponent(t);
	Drawable* drw = new Drawable(loseText, diedIMG, "youdiedtext", Globals::Layers::UI);
	loseText->addComponent(drw);
	graphics->addToDraw(loseText);
}

void Scene::hideLoseScreen() {
	if (gameOver) {
		if (loseBox) {
			graphics->removeFromDraw(loseBox);
			delete loseBox;
			loseBox = NULL;
		}
		if (loseText) {
			graphics->removeFromDraw(loseText);
			delete loseText;
			loseText = NULL;
		}
		gameOver = false;
		
		
	}
}

void Scene::showControlsScreen() {
	if (controlsWindowOn) return;
	controlsWindowOn = true;
	controlsScreen = new Entity();
	Transform* t = new Transform(controlsScreen, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, CameraSystem::posX, CameraSystem::posY);
	controlsScreen->addComponent(t);
	Drawable* drw = new Drawable(controlsScreen, infoIMG, "infoscreen", Globals::UI);
	controlsScreen->addComponent(drw);
	graphics->addToDraw(controlsScreen);
}

void Scene::hideControlsScreen() {
	controlsWindowOn = false;
	graphics->removeFromDraw(controlsScreen);
	delete controlsScreen;
	controlsScreen = NULL;
	SDL_DestroyTexture(infoIMG);
	infoIMG = NULL;
}

void Scene::startScene() {
	gameStarted = true;
	hideStartScreen();
	wait(1, "intro text");
}

void Scene::showStartScreen() {
	if (gameStarted) return;
	mainMenu = new Entity();
	Transform* t = new Transform(mainMenu, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, CameraSystem::posX, CameraSystem::posY);
	mainMenu->addComponent(t);
	Drawable* drw = new Drawable(mainMenu, startIMG, "startmenu", Globals::UI);
	mainMenu->addComponent(drw);
	graphics->addToDraw(mainMenu);
}

void Scene::hideStartScreen() {
	graphics->removeFromDraw(mainMenu);
	delete mainMenu;
	mainMenu = NULL;
	SDL_DestroyTexture(startIMG);
	startIMG = NULL;
}

void Scene::showEndScreen() {
	endScreen = new Entity();
	Transform* t = new Transform(endScreen, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, CameraSystem::posX, CameraSystem::posY);
	endScreen->addComponent(t);
	Drawable* drw = new Drawable(endScreen, endIMG, "endscreen", Globals::UI);
	endScreen->addComponent(drw);
	graphics->addToDraw(endScreen);
}
