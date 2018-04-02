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
	
	for (int i = 0; i < Globals::Layers::END_MARKER; i++) {
		std::vector<std::vector<Entity*>> layer;
		mapDrawQueue.push_back(layer);
	}

	for (int i = 0; i < mapMatrix.size(); i++) {
		std::vector<Entity*> row;
		for (int j = 0; j < mapMatrix[i].size(); j++) {
			for (auto entity : mapMatrix[i][j]) {
				Drawable* curDrawable = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
				mapDrawQueue[curDrawable->layer][i][j] = entity;
				// ^ TEST IF ABOVE WORKS //TODO
			}

		}
	}

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

	renderMap();

	// RENDERS GAME OBJECTS

	renderObjects();

	SDL_RenderPresent(renderer);
}

void Graphics::renderObjects() {
	int numOfRows = (int)objectDrawQueue.size();
	for (int i = 0; i < numOfRows; i++) {
		int numOfColumns = (int)objectDrawQueue[i].size();
		for (int j = 0; j < numOfColumns; j++) {
			
			Entity* curEntity = objectDrawQueue[i][j];
			Drawable* curDrawable = (Drawable*)objectDrawQueue[i][j]->findComponent(ComponentType::DRAWABLE);
			Transform* curTransform = (Transform*)objectDrawQueue[i][j]->findComponent(ComponentType::TRANSFORM);
			
			// Checks whether something is outside the camera and does not draw it
			if (curTransform->globalPosX + Globals::TILE_SIZE < Camera::posX || curTransform->globalPosY + Globals::TILE_SIZE < Camera::posY || curTransform->globalPosX > Camera::posX + Globals::SCREEN_WIDTH || curTransform->globalPosY > Camera::posY + Globals::SCREEN_HEIGHT) continue;
			
			

			SDL_Rect localPos;
			localPos.h = curTransform->height;
			localPos.w = curTransform->width;
			localPos.x = (int)(curTransform->globalPosX - Camera::posX);
			localPos.y = (int)(curTransform->globalPosY - Camera::posY);
			
			

			if (curTransform->isRotated) {
				SDL_RenderCopyEx(renderer, curDrawable->image, curDrawable->srcRect, &localPos, curTransform->rotationAngle, &curTransform->rotationCenter, curDrawable->flip);
			}
			else {
				SDL_RenderCopy(renderer, curDrawable->image, curDrawable->srcRect, &localPos);
			}

			

			if (debug) {
				if (curEntity->hasComponent(ComponentType::COLLIDER)) {
					Collider* col = (Collider*)curEntity->findComponent(ComponentType::COLLIDER);
					SDL_Rect collider;
					collider.h = col->colBox.h;
					collider.w = col->colBox.w;
					collider.x = col->colBox.x;
					collider.y = col->colBox.y;
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
					SDL_RenderDrawRect(renderer, &collider);
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
				}
			}

		}
	}
}

void Graphics::renderMap() {

}
