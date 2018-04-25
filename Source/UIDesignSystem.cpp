#include "UIDesignSystem.h"

SDL_Renderer* UIDesignSystem::renderer;
Graphics* UIDesignSystem::graphics;
SDL_Texture* UIDesignSystem::healthBarFull;
SDL_Texture* UIDesignSystem::healthBarEmpty;
unordered_map<Entity*, Entity*> UIDesignSystem::emptyBars;
unordered_map<Entity*, Entity*> UIDesignSystem::fullBars;
int UIDesignSystem::healthbarHeight;
int UIDesignSystem::healthbarWidth;

bool UIDesignSystem::isHealthShowing(Entity * entity) {
	if (emptyBars.find(entity) != emptyBars.end()) {
		return true;
	}
	return false;
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
}

void UIDesignSystem::setup(SDL_Renderer * renderer, Graphics * graphics) {
	UIDesignSystem::renderer = renderer;
	UIDesignSystem::graphics = graphics;
	healthBarFull = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "healthBarFull.png");
	healthBarEmpty = IMG_LoadTexture(renderer, ASSET_DIR UI_DIR "healthBarEmpty.png");
	SDL_QueryTexture(healthBarFull, NULL, NULL, &healthbarWidth, &healthbarHeight);
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
}
