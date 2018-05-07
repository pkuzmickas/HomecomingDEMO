#include "Scene.h"

Scene::~Scene() {
	if(player) delete player;
	MapSystem::deleteMap();
	DialogueSystem::cleanup();
	UIDesignSystem::cleanup();
	SDL_DestroyTexture(diedIMG);
	if (loseBox) delete loseBox;
	if (loseText) delete loseText;
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
		graphics->removeFromDraw(loseBox);
		graphics->removeFromDraw(loseText);
		gameOver = false;
		delete loseBox;
		loseBox = NULL;
		delete loseText;
		loseText = NULL;
	}
}
