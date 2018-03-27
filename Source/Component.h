#pragma once
#include <SDL.h>
#include "Globals.h"
#include <string>
#include "Camera.h"
enum ComponentType {
	NONE = 0,
	INPUT = 1 << 0,
	DRAWABLE = 1 << 1,
	MOVEMENT = 1 << 2,
	COLLIDER = 1 << 3,
	ANIMATION = 1 << 4,
	ABILITIES = 1 << 5,
	AI = 1 << 6,
	TRANSFORM = 1 << 7
};

class Entity;

class Component {
public:
	ComponentType type;
	bool enabled;
	Entity* owner;
	Component(Entity* owner) { this->owner = owner; enabled = true; }
};

class Transform : public Component {
public:
	SDL_Rect transformRect;
	float globalPosX;
	float globalPosY;
	bool isRotated;
	double rotationAngle;
	SDL_Point rotationCenter;
	Transform(Entity* owner, Camera* camera, int width = Globals::TILE_SIZE, int height = Globals::TILE_SIZE, float globalPosX = 0, float globalPosY = 0) : Component(owner) {
		this->globalPosX = globalPosX;
		this->globalPosY = globalPosY;
		isRotated = false;
		rotationAngle = 0;
		rotationCenter.x = 0;
		rotationCenter.y = 0;
		transformRect.w = width;
		transformRect.h = height;
		transformRect.x = globalPosX - camera->posX;
		transformRect.y = globalPosY - camera->posY;
		type = TRANSFORM;
	}
	void rotate(double rotationAngle, SDL_Point* rotationCenter = NULL) {
		isRotated = true;
		this->rotationAngle = rotationAngle;
		this->rotationCenter = *rotationCenter;
	}
};

class Input : public Component {
public:
	virtual void update(float deltaTime) = 0;
	Input(Entity* owner) : Component(owner) {
		type = INPUT;
	}
};

class Drawable : public Component {
public:

	SDL_Texture* image;
	SDL_Rect* srcRect;
	std::string ID;
	Globals::Layers layer;
	SDL_RendererFlip flip;
	Drawable(Entity* owner, SDL_Texture* image, std::string ID, Globals::Layers layer, SDL_Rect* srcRect = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) : Component(owner) {
		this->image = image;
		this->ID = ID;
		this->layer = layer;
		this->srcRect = srcRect;
		this->flip = flip;
		type = DRAWABLE;
	}
	~Drawable() {
		SDL_DestroyTexture(image);
		if (srcRect) delete srcRect;
	}

};

class Movement : public Component {
public:
	float velX;
	float velY;
	virtual void update(float deltaTime) = 0;
	Movement(Entity* owner, float velX = 0, float velY = 0) : Component(owner) {
		this->velX = velX;
		this->velY = velY;
		type = MOVEMENT;
	}
};

class Animation : public Component {
public:
	bool animating;
	SDL_Texture* spritesheet;
	SDL_Rect srcRect;
	int elapsedTime;
	int requiredTime;
	virtual void update(float deltaTime) = 0;
	Animation(Entity* owner, SDL_Texture* spritesheet) : Component(owner) {
		srcRect = { 0, 0, 0, 0 };
		elapsedTime = 0;
		requiredTime = 100;
		type = ANIMATION;
		this->spritesheet = spritesheet;
	}
};

class Collider : public Component {
public:
	enum ColliderType {
		NORMAL,
		ONLY_BLOCK_WALKING
	};
	ColliderType colType;
	SDL_Rect colBox;
	Collider(Entity* owner) : Component(owner) {
		type = COLLIDER;
		
	}
};

class Abilities : public Component {
	virtual void update(float deltaTime) = 0;
	Abilities(Entity* owner) : Component(owner) {
		type = ABILITIES;
	}
};
