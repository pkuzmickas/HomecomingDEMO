#include "Graphics.h"

void Graphics::addToDraw(Entity * entity) {
	Drawable* drawableComponent = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
	if (!drawableComponent) {
		std::cout << "Entity does not have a drawable component! Did not add to the draw queue." << std::endl;
		return;
	} 
	entityDrawQueue[drawableComponent->layer].push_back(entity);
}

Graphics::Graphics(SDL_Renderer * renderer) {
	this->renderer = renderer;
	for (int i = 0; i < Globals::Layers::END_MARKER; i++) {
		std::vector<Entity*> layer;
		entityDrawQueue.push_back(layer);
	}
}

void Graphics::render()
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < entityDrawQueue.size(); i++) {
		for (int j = 0; j < entityDrawQueue[i].size(); j++) {
			Entity* curEntity = entityDrawQueue[i][j];
			Drawable* curDrawable = (Drawable*)entityDrawQueue[i][j]->findComponent(ComponentType::DRAWABLE);  
			Transform* curTransform = (Transform*)entityDrawQueue[i][j]->findComponent(ComponentType::TRANSFORM);

			if (curTransform->isRotated) {

				SDL_RenderCopyEx(renderer, curDrawable->image, curDrawable->srcRect, &curTransform->transformRect, curTransform->rotationAngle, &curTransform->rotationCenter, curDrawable->flip);
			}
			else {
				SDL_RenderCopy(renderer, curDrawable->image, curDrawable->srcRect, &curTransform->transformRect);
			}

		}
	}

	SDL_RenderPresent(renderer);
}
