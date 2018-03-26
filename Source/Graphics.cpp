#include "Graphics.h"

void Graphics::addToDraw(Entity * entity) {
	Drawable* drawableComponent = (Drawable*)entity->findComponent(ComponentType::DRAWABLE);
	if (!drawableComponent) {
		std::cout << "Entity does not have a drawable component! Did not add to the draw queue." << std::endl;
		return;
	} 
	drawQueue[drawableComponent->layer].push_back(entity);
}

Graphics::Graphics(SDL_Renderer * renderer) {
	this->renderer = renderer;
	for (int i = 0; i < Globals::Layers::END_MARKER; i++) {
		std::vector<Entity*> layer;
		drawQueue.push_back(layer);
	}
}

void Graphics::render()
{
	SDL_RenderClear(renderer);

	for (int i = 0; i < drawQueue.size(); i++) {
		for (int j = 0; j < drawQueue[i].size(); j++) {
			Entity* curEntity = drawQueue[i][j];
			Drawable* curDrawable;
			/*if (drawQueue[i][j]->) {

				SDL_RenderCopyEx(renderer, drawQueue[i][j]->image, drawQueue[i][j]->srcRect, drawQueue[i][j]->posRect, drawQueue[i][j]->angle, drawQueue[i][j]->center, drawQueue[i][j]->flip);
			}
			else {
				SDL_RenderCopy(renderer, drawQueue[i][j]->image, drawQueue[i][j]->srcRect, drawQueue[i][j]->posRect);
			}*/

		}
	}

	SDL_RenderPresent(renderer);
}
