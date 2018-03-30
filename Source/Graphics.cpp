#include "Graphics.h"

void Graphics::addToDraw(Entity * entity) {
	Drawable* drawableComponent = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
	if (!drawableComponent) {
		std::cout << "Entity does not have a drawable component! Did not add to the draw queue." << std::endl;
		return;
	} 
	objectDrawQueue[drawableComponent->layer].push_back(entity);
}

void Graphics::addMap(std::vector<std::vector<std::vector<Entity*>>> mapMatrix) {
	std::vector<std::vector<Entity*>> map;
	for (int i = 0; i < Globals::Layers::END_MARKER; i++) {
		std::vector<Entity*> layer;
		map.push_back(layer);
	}
	for (auto row : mapMatrix) {
		for (auto col : row) {
			for (auto entity : col) {
				Drawable* curDrawable = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
				map[(int)(curDrawable->layer)].push_back(entity);
			}
		}
	}

	mapDrawQueue = map;
}

Graphics::Graphics(SDL_Renderer * renderer) {
	this->renderer = renderer;
	for (int i = 0; i < Globals::Layers::END_MARKER; i++) {
		std::vector<Entity*> layer;
		objectDrawQueue.push_back(layer);
	}
}

void Graphics::render()
{
	SDL_RenderClear(renderer);

	// RENDERS MAP

	renderDrawQueue(mapDrawQueue);

	// RENDERS GAME OBJECTS

	renderDrawQueue(objectDrawQueue);

	SDL_RenderPresent(renderer);
}

void Graphics::renderDrawQueue(std::vector<std::vector<Entity*>> drawQueue) {
	for (int i = 0; i < (int)drawQueue.size(); i++) {
		for (int j = 0; j < (int)drawQueue[i].size(); j++) {
			Entity* curEntity = drawQueue[i][j];
			Drawable* curDrawable = (Drawable*)drawQueue[i][j]->findComponent(ComponentType::DRAWABLE);
			Transform* curTransform = (Transform*)drawQueue[i][j]->findComponent(ComponentType::TRANSFORM);

			if (curTransform->isRotated) {

				SDL_RenderCopyEx(renderer, curDrawable->image, curDrawable->srcRect, &curTransform->transformRect, curTransform->rotationAngle, &curTransform->rotationCenter, curDrawable->flip);
			}
			else {
				SDL_RenderCopy(renderer, curDrawable->image, curDrawable->srcRect, &curTransform->transformRect);
			}

		}
	}
}
