#include "Graphics.h"

void Graphics::addToDraw(Entity * entity) {
	Drawable* drawableComponent = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
	if (!drawableComponent) {
		std::cout << "Entity does not have a drawable component! Did not add to the draw queue." << std::endl;
		return;
	}
	Transform* transformComponent = (Transform*)entity->findComponent(ComponentType::TRANSFORM);
	if (!transformComponent) {
		std::cout << "Entity does not have a transform component! Did not add to the draw queue." << std::endl;
		return;
	}
	Sprite sprite;
	sprite.drawable = drawableComponent;
	sprite.transform = transformComponent;
	objectDrawQueue[drawableComponent->layer].push_back(sprite);
}

void Graphics::addMap(std::vector<std::vector<std::vector<Entity*>>> mapMatrix) {
	
	mapRows = mapMatrix.size();
	mapCols = mapMatrix[0].size();
	for (int i = 0; i < mapMatrix.size(); i++) {
		for (int j = 0; j < mapMatrix[i].size(); j++) {
			for (auto entity : mapMatrix[i][j]) {
				Drawable* curDrawable = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
				Transform* curTransform = (Transform*)entity->findComponent(ComponentType::TRANSFORM);
				Sprite sprite;
				sprite.drawable = curDrawable;
				sprite.transform = curTransform;
				mapDrawQueue[curDrawable->layer].push_back(sprite);
			}
		}
	}

}

Graphics::Graphics(SDL_Renderer * renderer) {
	this->renderer = renderer;
	for (int i = 0; i < Globals::Layers::END_MARKER; i++) {
		std::vector<Sprite> layer;
		objectDrawQueue.push_back(layer);
		if (i < Globals::Layers::UI) {
			mapDrawQueue.push_back(layer);
		}
	}
}

void Graphics::render()
{
	SDL_RenderClear(renderer);

	for (int layer = 0; layer < Globals::Layers::END_MARKER; layer++) {
		// RENDERS MAP
		if (layer < 1) { // now need to add exceptions for other layers
			// Wild algorithm to reduce number of loops and draws. Copyright Paulius is awesome.
			int xBound = Camera::posX / Globals::TILE_SIZE + Globals::SCREEN_WIDTH/Globals::TILE_SIZE +1;
			int yBound = Camera::posY / Globals::TILE_SIZE + Globals::SCREEN_HEIGHT / Globals::TILE_SIZE+1;
			int xStart = Camera::posX / Globals::TILE_SIZE;
			int yStart = Camera::posY / Globals::TILE_SIZE;
			int startLoopX = xStart + mapCols * yStart;
			int stopLoopX = xBound + mapCols * yStart;
			int stopLoopAt = xStart + mapCols * yBound;

			int curSpriteNr = startLoopX;
			int curY = yStart;
			int sum = 0;
			while (curSpriteNr!= stopLoopAt) {

				Transform* curTransform = mapDrawQueue[layer][curSpriteNr].transform;
				Drawable* curDrawable = mapDrawQueue[layer][curSpriteNr].drawable;
				sum++;
				SDL_Rect localPos;
				localPos.h = curTransform->height;
				localPos.w = curTransform->width;
				localPos.x = (int)(curTransform->globalPosX - Camera::posX);
				localPos.y = (int)(curTransform->globalPosY - Camera::posY);

				SDL_RenderCopy(renderer, curDrawable->image, curDrawable->srcRect, &localPos);

				curSpriteNr++;
				if (curSpriteNr == stopLoopX) {
					curSpriteNr = xStart + mapCols * ++curY;
					stopLoopX += mapCols;
				}
				
			}
		}
		/*
		SDL_Rect localPos;
		localPos.h = curTransform->height;
		localPos.w = curTransform->width;
		localPos.x = (int)(curTransform->globalPosX - Camera::posX);
		localPos.y = (int)(curTransform->globalPosY - Camera::posY);

		SDL_RenderCopy(renderer, curDrawable->image, curDrawable->srcRect, &localPos);

		/*if (debug) {
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
		}*/
		// DRAW OBJECTS

		/*for (int j = 0; j < (int)objectDrawQueue[layer].size(); j++) {

			Entity* curEntity = objectDrawQueue[layer][j];
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

		}*/


	}

	SDL_RenderPresent(renderer);
}
