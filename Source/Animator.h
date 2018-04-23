#pragma once
#include "Entity.h"
#include "Collider.h"
class Animator : public Component {
public:
	enum LookDirection {
		DOWN,
		LEFT,
		RIGHT,
		UP
	};
	struct Animation {
		std::string name;
		std::vector<int> spriteSequence;
		int speed;
		bool loop;
		int spritesInRow;
		Animation() {};
		Animation(std::string name, std::vector<int> spriteSequence, int speed, bool loop, int spritesInRow = 0) {
			this->name = name;
			this->spriteSequence = spriteSequence;
			this->speed = speed;
			this->loop = loop;
			this->spritesInRow = spritesInRow;
		}
	};
	bool isAnimating() { return animating; }
	std::string curAnimName() { return curAnim.name; }
	void addAnimation(Animation animation);
	void playAnimation(std::string name, bool loop = false);
	void stopAnimation();
	virtual void update(float deltaTime);
	Animator(Entity* owner) : Component(owner, true) {
		Drawable* drawable = (Drawable*)owner->findComponent(ComponentType::DRAWABLE);
		curSrcRect = drawable->srcRect;
		SDL_QueryTexture(drawable->image, NULL, NULL, &curSSW, &curSSH);
		type = ANIMATOR;
	}
protected:
	SDL_Rect * curSrcRect;
private:
	int elapsedTime;
	std::unordered_map<std::string, Animation> animations;
	Animation curAnim;
	int nextSeqID;
	int curSSW = 0;
	int curSSH = 0;
	bool animating;
};