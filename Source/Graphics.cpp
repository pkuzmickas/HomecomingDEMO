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

	for (int i = 0; i < mapMatrix.size(); i++) {
		std::vector<std::map<Globals::Layers, Entity*>> row;
		for (int j = 0; j < mapMatrix[i].size(); j++) {
			std::map<Globals::Layers, Entity*> col;
			row.push_back(col);
		}
		mapDrawQueue.push_back(row);
	}
	
	for (int i = 0; i < mapMatrix.size(); i++) {
		for (int j = 0; j < mapMatrix[i].size(); j++) {
			for (auto entity : mapMatrix[i][j]) {
				Drawable* curDrawable = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
				mapDrawQueue[i][j][curDrawable->layer] = entity;
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

	if(!mapDrawQueue.empty()) renderMap();

	// RENDERS GAME OBJECTS

	renderObjects();

	SDL_RenderPresent(renderer);
}

void Graphics::renderObjects() {
	for (int i = 0; i < (int)objectDrawQueue.size(); i++) {
		for (int j = 0; j < (int)objectDrawQueue[i].size(); j++) {

			Entity* curEntity = objectDrawQueue[i][j];
			Drawable* curDrawable = (Drawable*)curEntity->findComponent(ComponentType::DRAWABLE);
			Transform* curTransform = (Transform*)curEntity->findComponent(ComponentType::TRANSFORM);

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
	for (int layer = 0; layer < Globals::Layers::UI; layer++) {
		for (int i = Camera::posY / Globals::TILE_SIZE; i < Camera::posY / Globals::TILE_SIZE + Globals::SCREEN_HEIGHT / Globals::TILE_SIZE + 1; i++) {
			if (i >= mapDrawQueue.size()) break;
			for (int j = Camera::posX / Globals::TILE_SIZE; j < Camera::posX / Globals::TILE_SIZE + Globals::SCREEN_WIDTH / Globals::TILE_SIZE + 1; j++) {
				if (j >= mapDrawQueue[i].size()) break;
				if(mapDrawQueue[i][j].find((Globals::Layers)layer) != mapDrawQueue[i][j].end()) {

					Entity* curEntity = mapDrawQueue[i][j][(Globals::Layers)layer];
					if (curEntity == NULL) {
						continue;
					}

					Drawable* curDrawable = (Drawable*)curEntity->findComponent(ComponentType::DRAWABLE);
					Transform* curTransform = (Transform*)curEntity->findComponent(ComponentType::TRANSFORM);

					SDL_Rect localPos;
					localPos.h = curTransform->height;
					localPos.w = curTransform->width;
					localPos.x = (int)(curTransform->globalPosX - Camera::posX);
					localPos.y = (int)(curTransform->globalPosY - Camera::posY);

					SDL_RenderCopy(renderer, curDrawable->image, curDrawable->srcRect, &localPos);

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
	}
}
