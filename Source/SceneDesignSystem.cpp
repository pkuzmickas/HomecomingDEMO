#include "SceneDesignSystem.h"

int SceneDesignSystem::treesSpawned = 0;

void SceneDesignSystem::checkFontSizeOnText(std::string text, int fontSize, int * width, int * height) {
	TTF_Font* temp = TTF_OpenFont(ASSET_DIR FONT_PATH, fontSize);
	TTF_SizeText(temp, text.c_str(), width, height); 
	TTF_CloseFont(temp);
}

Entity* SceneDesignSystem::createTree(int posX, int posY, Globals::Layers layer, SDL_Texture * texture, SDL_Rect* colOffset, SDL_Rect* srcRect) {
	Entity* tree = new Entity();
	int width, height;
	if (!srcRect) {
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}
	else {
		width = srcRect->w;
		height = srcRect->h;
	}
	Transform* transform = new Transform(tree, width, height, posX, posY);
	tree->addComponent(transform);
	Drawable* drawable = new Drawable(tree, texture, "tree" + treesSpawned, layer, srcRect);
	tree->addComponent(drawable);
	Collider* collider = NULL;
	if (!colOffset) {
		collider = new Collider(tree); 
	}
	else {
		collider = new Collider(tree, Collider::NORMAL, colOffset->x, colOffset->y, colOffset->w, colOffset->h);
	}
	tree->addComponent(collider);
	CollisionSystem::collidersInScene.push_back(collider);
	Animator* animator = new Animator(tree);
	tree->addComponent(animator);
	Animator::Animation hit("treeHit", { 0, 1, 2 }, 30);
	animator->addAnimation(hit);
	Animator::Animation hit2("treeHit2", { 3, 4, 5 }, 30);
	animator->addAnimation(hit2);
	Stats* treeStats = new Stats(tree);
	treeStats->totalHealth = 100;
	treeStats->curHealth = 100;
	tree->addComponent(treeStats);
	return tree;
}

Entity * SceneDesignSystem::createRect(int posX, int posY, int width, int height, Globals::Layers layer, bool fill)
{
	Entity* rect = new Entity();
	Transform* transform = new Transform(rect, width, height, posX, posY);
	rect->addComponent(transform);
	Drawable* drawable = new Drawable(rect, layer, fill);
	rect->addComponent(drawable);
	return rect;
}

Entity * SceneDesignSystem::createText(std::string text, int posX, int posY, int fontSize, SDL_Color color, Globals::Layers layer, SDL_Renderer* renderer)
{
	TTF_Font* textFont = TTF_OpenFont(ASSET_DIR FONT_PATH, fontSize);
	SDL_Surface* textSurface = TTF_RenderText_Blended(textFont, text.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(textFont);
	Entity* textEntity = new Entity();
	Transform* transform = new Transform(textEntity, textSurface->w, textSurface->h, posX, posY);
	textEntity->addComponent(transform);
	Drawable* drawable = new Drawable(textEntity, textTexture, "textDesign", layer);
	textEntity->addComponent(drawable);
	return textEntity;
}

void SceneDesignSystem::cleanupText(Entity* text) {
	if (text) {
		SDL_Texture* texture = ((Drawable*)text->findComponent(ComponentType::DRAWABLE))->image;
		SDL_DestroyTexture(texture);
		delete text;
		text = NULL;
	}
	
}
