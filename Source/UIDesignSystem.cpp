#include "UIDesignSystem.h"

SDL_Renderer* UIDesignSystem::renderer;
Graphics* UIDesignSystem::graphics;
SDL_Texture* UIDesignSystem::healthBarFull;
SDL_Texture* UIDesignSystem::healthBarEmpty;
SDL_Texture* UIDesignSystem::bloodshotIMG;
SDL_Texture* UIDesignSystem::playerHealthBarEmpty;
SDL_Texture* UIDesignSystem::playerHealthBarFull;
SDL_Texture* UIDesignSystem::attackUI;
Entity* UIDesignSystem::playerEmptyBar = NULL;
Entity* UIDesignSystem::playerFullBar = NULL;
unordered_map<Entity*, Entity*> UIDesignSystem::emptyBars;
unordered_map<Entity*, Entity*> UIDesignSystem::fullBars;
vector<Entity*> UIDesignSystem::bloodShots;
int UIDesignSystem::healthbarHeight;
int UIDesignSystem::healthbarWidth;
int UIDesignSystem::playerHealthbarWidth;
int UIDesignSystem::playerHealthbarHeight;
bool UIDesignSystem::showingPlayerUI = false;
Entity* UIDesignSystem::playerEntity = NULL;
vector<Entity*> UIDesignSystem::attackIcons;

bool UIDesignSystem::isHealthShowing(Entity * entity) {
	if (emptyBars.find(entity) != emptyBars.end()) {
		return true;
	}
	return false;
}

void UIDesignSystem::createBloodshot(Entity* entity) {
	Transform* objectTransform = (Transform*)entity->findComponent(ComponentType::TRANSFORM);
	Entity* blood = new Entity();
	bloodShots.push_back(blood);
	SDL_Rect* srcRect = new SDL_Rect();
	srcRect->x = 0;
	srcRect->y = 0;
	srcRect->w = 215;
	srcRect->h = 200;
	Transform* t = new Transform(blood, srcRect->w, srcRect->h, objectTransform->globalPosX + objectTransform->width/2 - srcRect->w/2, objectTransform->globalPosY + objectTransform->height / 2 - srcRect->h / 2);
	blood->addComponent(t);
	Drawable* drw = new Drawable(blood, bloodshotIMG, "blood", Globals::Layers::UI, srcRect);
	blood->addComponent(drw);
	Animator* anim = new Animator(blood);
	blood->addComponent(anim);
	Animator::Animation a("bloodshot", { 0, 1, 2 }, 40);
	anim->addAnimation(a);
	graphics->addToDraw(blood);
	anim->playAnimation("bloodshot");
}

void UIDesignSystem::cleanup() {
	for (auto it : emptyBars) {
		delete it.second;
	}
	for (auto it : fullBars) {
		delete it.second;
	}
	emptyBars.clear();
	fullBars.clear();
	SDL_DestroyTexture(healthBarFull);
	SDL_DestroyTexture(healthBarEmpty);
	SDL_DestroyTexture(bloodshotIMG);
	SDL_DestroyTexture(playerHealthBarEmpty);
	SDL_DestroyTexture(playerHealthBarFull);
	if (playerFullBar) {
		delete playerFullBar;
		playerFullBar = NULL;
	}
	if (playerEmptyBar) {
		delete playerEmptyBar;
		playerEmptyBar = NULL;
	}
	showingPlayerUI = false;
}

void UIDesignSystem::setup(SDL_Renderer * renderer, Graphics * graphics) {
	UIDesignSystem::renderer = renderer;
	UIDesignSystem::graphics = graphics;
	healthBarFull = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "healthBarFull.png");
	healthBarEmpty = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "healthBarEmpty.png");
	bloodshotIMG = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "blood.png");
	attackUI = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "attackIcons.png");
	playerHealthBarEmpty = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "playerHealthBarEmpty.png");
	playerHealthBarFull = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "playerHealthBarFull.png");
	SDL_QueryTexture(healthBarFull, NULL, NULL, &healthbarWidth, &healthbarHeight);
	SDL_QueryTexture(playerHealthBarFull, NULL, NULL, &playerHealthbarWidth, &playerHealthbarHeight);
}

void UIDesignSystem::showHealth(Entity * entity) {
	Transform* entityTransform = (Transform*)entity->findComponent(ComponentType::TRANSFORM);
	Drawable* entityDrawable = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);

	Entity* emptyBar = new Entity();
	Transform* t = new Transform(emptyBar, healthbarWidth, healthbarHeight, entityTransform->globalPosX + entityTransform->width / 2 - healthbarWidth / 2, entityTransform->globalPosY - healthbarHeight - 10);
	emptyBar->addComponent(t);
	Drawable* d = new Drawable(emptyBar, healthBarEmpty, "emptyHealthbarFor" + entityDrawable->ID, Globals::Layers::UI);
	emptyBar->addComponent(d);
	graphics->addToDraw(emptyBar);
	emptyBars[entity] = emptyBar;

	Entity* fullBar = new Entity();
	t = new Transform(fullBar, healthbarWidth, healthbarHeight, entityTransform->globalPosX + entityTransform->width / 2 - healthbarWidth / 2, entityTransform->globalPosY - healthbarHeight - 10);
	fullBar->addComponent(t);
	SDL_Rect* src = new SDL_Rect();
	src->h = healthbarHeight;
	src->w = healthbarWidth;
	src->x = 0;
	src->y = 0;
	d = new Drawable(fullBar, healthBarFull, "fullHealthbarFor" + entityDrawable->ID, Globals::Layers::UI, src);
	fullBar->addComponent(d);
	graphics->addToDraw(fullBar);
	fullBars[entity] = fullBar;
}

void UIDesignSystem::hideHealth(Entity * entity) {
	if (emptyBars.find(entity) != emptyBars.end()) {
		graphics->removeFromDraw(emptyBars[entity]);
		graphics->removeFromDraw(fullBars[entity]);
		delete emptyBars[entity];
		delete fullBars[entity];
		emptyBars.erase(entity);
		fullBars.erase(entity);
	}
	
}


void UIDesignSystem::showPlayerUI(Entity * player) {
	if (showingPlayerUI) return;
	if (!playerEntity) playerEntity = player;
	showingPlayerUI = true;
	playerEmptyBar = new Entity();
	Transform* t = new Transform(playerEmptyBar, playerHealthbarWidth, playerHealthbarHeight, CameraSystem::posX + Globals::SCREEN_WIDTH/20, CameraSystem::posY + Globals::SCREEN_HEIGHT/16);
	playerEmptyBar->addComponent(t);
	Drawable* d = new Drawable(playerEmptyBar, playerHealthBarEmpty, "emptyPlayerHealthbar", Globals::Layers::UI);
	playerEmptyBar->addComponent(d);
	graphics->addToDraw(playerEmptyBar);

	playerFullBar = new Entity();
	t = new Transform(playerFullBar, playerHealthbarWidth, playerHealthbarHeight, CameraSystem::posX + Globals::SCREEN_WIDTH / 20, CameraSystem::posY + Globals::SCREEN_HEIGHT / 16);
	playerFullBar->addComponent(t);
	SDL_Rect* src = new SDL_Rect();
	src->h = playerHealthbarHeight;
	src->w = playerHealthbarWidth;
	src->x = 0;
	src->y = 0;
	d = new Drawable(playerFullBar, playerHealthBarFull, "fullPlayerHealthbar", Globals::Layers::UI, src);
	playerFullBar->addComponent(d);
	graphics->addToDraw(playerFullBar);

	for (int i = 0; i < 3; i++) {
		Entity* attIcon = new Entity();
		SDL_Rect* src = new SDL_Rect();
		src->h = 64;
		src->w = 64;
		src->x = 0;
		src->y = i * src->h;
		Transform* t = new Transform(attIcon, src->w, src->h, CameraSystem::posX + Globals::SCREEN_WIDTH - 100 - (3-i)*64 - 20, CameraSystem::posY + Globals::SCREEN_HEIGHT - 100 - 64 - 20);
		attIcon->addComponent(t);
		Drawable* d = new Drawable(attIcon, attackUI, "playerAttackUIElement", Globals::Layers::UI, src);
		attIcon->addComponent(d);
		graphics->addToDraw(attIcon);
		attackIcons.push_back(attIcon);
	}
}

void UIDesignSystem::hidePlayerUI() {
	if (showingPlayerUI) {
		graphics->removeFromDraw(playerFullBar);
		graphics->removeFromDraw(playerEmptyBar);
		for (auto el : attackIcons) {
			graphics->removeFromDraw(el);
		}
		showingPlayerUI = false;
	}
}

void UIDesignSystem::activateAttack(int id) {
	Drawable* drw = (Drawable*)attackIcons[id]->findComponent(ComponentType::DRAWABLE);
	drw->srcRect->x = 0;
}
void UIDesignSystem::deactivateAttack(int id) {
	Drawable* drw = (Drawable*)attackIcons[id]->findComponent(ComponentType::DRAWABLE);
	drw->srcRect->x = drw->srcRect->w;
}

void UIDesignSystem::update(float deltaTime) {
	for (auto it : emptyBars) {
		Transform* gameobjectTransform = (Transform*)it.first->findComponent(ComponentType::TRANSFORM);
		Transform* barTransform = (Transform*)it.second->findComponent(ComponentType::TRANSFORM);
		barTransform->globalPosX = gameobjectTransform->globalPosX + gameobjectTransform->width / 2 - healthbarWidth / 2;
		barTransform->globalPosY = gameobjectTransform->globalPosY - healthbarHeight - 10;
	}
	for (auto it : fullBars) {
		Transform* gameobjectTransform = (Transform*)it.first->findComponent(ComponentType::TRANSFORM);
		Transform* barTransform = (Transform*)it.second->findComponent(ComponentType::TRANSFORM);
		Drawable* barDrawable = (Drawable*)it.second->findComponent(ComponentType::DRAWABLE);
		Stats* gameobjectStats = (Stats*)it.first->findComponent(ComponentType::STATS);
		barTransform->globalPosX = gameobjectTransform->globalPosX + gameobjectTransform->width / 2 - healthbarWidth / 2;
		barTransform->globalPosY = gameobjectTransform->globalPosY - healthbarHeight - 10;
		float healthPercentage = (float)gameobjectStats->curHealth / (float)gameobjectStats->totalHealth;
		float newWidth = (float)healthbarWidth * healthPercentage;
		barDrawable->srcRect->w = newWidth;
		barTransform->width = newWidth;
		
	}
	for (int i=0; i<(int)bloodShots.size(); i++) {
		Entity* blood = bloodShots[i];
		blood->update(deltaTime);
		Animator* anim = (Animator*)blood->findComponent(ComponentType::ANIMATOR);
		if (!anim->isAnimating()) {
			iter_swap(bloodShots.begin() + i, bloodShots.begin() + bloodShots.size() - 1);
			bloodShots.pop_back();
			graphics->removeFromDraw(blood);
			delete blood;
		}
	}
	if (showingPlayerUI) {
		Transform* t1 = (Transform*)playerFullBar->findComponent(ComponentType::TRANSFORM);
		Transform* t2 = (Transform*)playerEmptyBar->findComponent(ComponentType::TRANSFORM);
		t1->globalPosX = CameraSystem::posX + Globals::SCREEN_WIDTH / 20;
		t1->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT / 16;
		t2->globalPosX = CameraSystem::posX + Globals::SCREEN_WIDTH / 20;
		t2->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT / 16;
		PlayerStats* ps = (PlayerStats*)playerEntity->findComponent(ComponentType::STATS);
		int headWidth = 50;
		float healthPercentage = (float)(ps->curHealth) / (float)(ps->totalHealth);
		float newWidth = headWidth + (float)(playerHealthbarWidth-50) * healthPercentage;
		Transform* barTransform = (Transform*)playerFullBar->findComponent(ComponentType::TRANSFORM);
		Drawable* barDrawable = (Drawable*)playerFullBar->findComponent(ComponentType::DRAWABLE);
		barDrawable->srcRect->w = newWidth;
		barTransform->width = newWidth;
		for (int i = 0; i < (int)attackIcons.size(); i++) {
			Entity* el = attackIcons[i];
			Transform* t = (Transform*)el->findComponent(ComponentType::TRANSFORM);
			//CameraSystem::posX + Globals::SCREEN_WIDTH - 100 - (3-i)*64, CameraSystem::posY + Globals::SCREEN_HEIGHT - 100 - 64
			t->globalPosX = CameraSystem::posX + Globals::SCREEN_WIDTH - 100 - (3 - i) * 80;
			t->globalPosY = CameraSystem::posY + Globals::SCREEN_HEIGHT - 50 - 64;
		}
	}
	
}
