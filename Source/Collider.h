#pragma once
#include "Entity.h"


#include <algorithm>
class Collider : public Component {
public:
	enum ColliderType {
		NONE,
		LOW,//attacks are not blocked
		NORMAL
	};
	ColliderType colType;
	SDL_Rect colBox; // X and Y are relative to the transform of the entity
	SDL_Rect offset;
	virtual void update(float deltaTime) {
		colBox.x = (int)(transform->globalPosX - CameraSystem::posX) + offset.x;
		colBox.y = (int)(transform->globalPosY - CameraSystem::posY) + offset.y;
	}
	Collider(Entity* owner, ColliderType colType = ColliderType::NORMAL, int XOffset = 0, int YOffset = 0, int width = 0, int height = 0) : Component(owner, true) {
		type = COLLIDER;
		this->colType = colType;
		offset.x = XOffset;
		offset.y = YOffset;
		transform = (Transform*)owner->findComponent(ComponentType::TRANSFORM);
		if (width == 0 && height == 0) {
			width = transform->width;
			height = transform->height;
		} 
		offset.w = width;
		offset.h = height;
		colBox.w = width;
		colBox.h = height;
	}
private:
	Transform * transform;
};