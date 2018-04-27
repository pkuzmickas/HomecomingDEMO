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
	sprite.entity = entity;
	objectDrawQueue[drawableComponent->layer].push_back(sprite);
}

bool Graphics::removeFromDraw(Entity * entity) {
	Drawable* drawable = (Drawable*) entity->findComponent(ComponentType::DRAWABLE);
	for(int i=0; i < (int)objectDrawQueue[drawable->layer].size(); i++) {
		if (objectDrawQueue[drawable->layer][i].entity == entity) {
			// Swaps the element with the matching name with the last one in the vector and then pops the vector
			iter_swap(objectDrawQueue[drawable->layer].begin() + i, objectDrawQueue[drawable->layer].begin() + objectDrawQueue[drawable->layer].size() - 1);
			objectDrawQueue[drawable->layer].pop_back();
			return true;
		}
	}
	cout << "COULD NOT REMOVE FROM DRAW" << endl;
	return false;
}

void Graphics::addMap(std::vector<std::vector<std::vector<Entity*>>> mapMatrix) {

	mapRows = mapMatrix.size();
	mapCols = mapMatrix[0].size();
	for (int i = 0; i < (int)mapMatrix.size(); i++) {
		std::vector<std::vector<Sprite>> row;
		for (int j = 0; j < (int)mapMatrix[i].size(); j++) {
			std::vector<Sprite > col;
			for (auto entity : mapMatrix[i][j]) {
				Drawable* curDrawable = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
				Transform* curTransform = (Transform*)entity->findComponent(ComponentType::TRANSFORM);
				Sprite sprite;
				sprite.drawable = curDrawable;
				sprite.transform = curTransform;
				sprite.entity = entity;
				col.push_back(sprite);
			}
			row.push_back(col);
		}
		mapMatrixWSprites.push_back(row);
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

	int xBound = (int)(CameraSystem::posX / Globals::TILE_SIZE + Globals::SCREEN_WIDTH / Globals::TILE_SIZE + 2);
	if (xBound > MapSystem::getWidth() / Globals::TILE_SIZE) xBound = MapSystem::getWidth() / Globals::TILE_SIZE;
	int yBound = (int)(CameraSystem::posY / Globals::TILE_SIZE + Globals::SCREEN_HEIGHT / Globals::TILE_SIZE + 2);
	if (yBound > MapSystem::getHeight() / Globals::TILE_SIZE) yBound = MapSystem::getHeight() / Globals::TILE_SIZE;
	int xStart = (int)(CameraSystem::posX / Globals::TILE_SIZE);
	int yStart = (int)(CameraSystem::posY / Globals::TILE_SIZE);
	
	// Checks the tiles needed to be drawn in regards to the camera and adds them to the vector
	for (int row = yStart; row < yBound; row++) {
		for (int col = xStart; col < xBound; col++) {
			for (auto sprite : mapMatrixWSprites[row][col]) {
				mapDrawQueue[sprite.drawable->layer].push_back(sprite);
			}
		}
	}

	// Renders the tiles and game objects
	for (int layer = 0; layer < Globals::Layers::END_MARKER; layer++) {
		if (layer < Globals::Layers::UI) {
			for (auto tileSprite : mapDrawQueue[layer]) {
				if(!debug)draw(tileSprite);
				tileSprite.entity->update(0); // need to update all of the  visible tiles in case the camera moves
			}

			// DEBUG CODE
			if (debug) {
				for (auto tileSprite : mapDrawQueue[layer]) {
					if (tileSprite.drawable->owner->hasComponent(ComponentType::COLLIDER)) {
						Collider* col = (Collider*)tileSprite.drawable->owner->findComponent(ComponentType::COLLIDER);
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
			// END OF DEBUG CODE

			mapDrawQueue[layer].clear();
			
		}
		for (auto gameObject : objectDrawQueue[layer]) {
			if (gameObject.drawable->isRect) {
				SDL_Rect localPos;
				localPos.h = gameObject.transform->height;
				localPos.w = gameObject.transform->width;
				localPos.x = (int)(gameObject.transform->globalPosX - CameraSystem::posX);
				localPos.y = (int)(gameObject.transform->globalPosY - CameraSystem::posY);
				if (gameObject.drawable->fill) {
					SDL_RenderFillRect(renderer, &localPos);
				}
				else {
					SDL_RenderDrawRect(renderer, &localPos);
				}
			}
			else {
				draw(gameObject);
			}

			// DEBUG CODE
			if (debug) {
				for (auto gameObject : objectDrawQueue[layer]) {
					if (gameObject.drawable->owner->hasComponent(ComponentType::COLLIDER)) {
						Collider* col = (Collider*)gameObject.drawable->owner->findComponent(ComponentType::COLLIDER);
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
			// END OF DEBUG CODE
		}
	}



	SDL_RenderPresent(renderer);
}

void Graphics::draw(Sprite sprite) {
	SDL_Rect localPos;
	localPos.h = sprite.transform->height;
	localPos.w = sprite.transform->width;
	localPos.x = (int)(sprite.transform->globalPosX - CameraSystem::posX);
	localPos.y = (int)(sprite.transform->globalPosY - CameraSystem::posY);
	if (!sprite.transform->isRotated) {
		SDL_RenderCopy(renderer, sprite.drawable->image, sprite.drawable->srcRect, &localPos);
	}
	else {
		SDL_RenderCopyEx(renderer, sprite.drawable->image, sprite.drawable->srcRect, &localPos, sprite.transform->rotationAngle, &sprite.transform->rotationCenter, sprite.drawable->flip);
	}
}

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