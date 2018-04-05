#include "Animator.h"

void Animator::addAnimation(Animation animation) {
	if (animations.find(animation.name) != animations.end()) {
		std::cout << "Animation '"<<animation.name<<"' already exists! Overwriting..." << std::endl;
	}
	animations[animation.name] = animation;
}

void Animator::playAnimation(std::string name) {
	if (animations.find(name) != animations.end()) {
		curAnim = animations[name];
		animating = true;
		nextSeqID = 0;
	}
	else {
		std::cout << "Animation '"<<name<<"' does not exist!" << std::endl;
	}
}

void Animator::update(float deltaTime) {
	if (animating) {
		if (SDL_GetTicks() - elapsedTime > curAnim.speed) {

			if (nextSeqID == curAnim.spriteSequence.size()) {
				if (!curAnim.loop) {
					curSrcRect = NULL;
					animating = false;
				}
				else {
					nextSeqID = 0;
				}

			}
			if (nextSeqID < curAnim.spriteSequence.size()) {
				elapsedTime = SDL_GetTicks();
				int spriteNr = curAnim.spriteSequence[nextSeqID++];
				if (!curSrcRect) {
					Drawable* drawable = (Drawable*)owner->findComponent(ComponentType::DRAWABLE);
					curSrcRect = drawable->srcRect;
					SDL_QueryTexture(drawable->image, NULL, NULL, &curSSH, &curSSW);
				}
				int spritesInRow = curSSW / curSrcRect->w;
				curSrcRect->x = (spriteNr % spritesInRow) * curSrcRect->w;
				curSrcRect->y = (spriteNr / spritesInRow) * curSrcRect->h;
			}
		}
	}
}
