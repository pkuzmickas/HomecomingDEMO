#pragma once
#include <SDL.h>
#include <string>
#include "CameraSystem.h"
#include <iostream>

enum ComponentType {
	NONE = 0,
	INPUT = 1 << 0,
	DRAWABLE = 1 << 1,
	MOVEMENT = 1 << 2,
	COLLIDER = 1 << 3,
	ANIMATOR = 1 << 4,
	ABILITIES = 1 << 5,
	AI = 1 << 6,
	TRANSFORM = 1 << 7,
	STATS = 1 << 8,
	TALKABLE = 1 << 9
};

class Entity;

class Component {
public:
	ComponentType type;
	bool enabled;
	bool updatable;
	virtual void update(float deltaTime) { std::cout << "Component's update function was not defined! TYPE:" << type << std::endl; };
	Entity* owner;
	Component(Entity* owner, bool updatable = false) { this->owner = owner; enabled = true; this->updatable = updatable; }
	virtual ~Component() {};
};

class Transform : public Component {
public:
	float globalPosX;
	float globalPosY;
	int width;
	int height;
	bool isRotated;
	double rotationAngle;
	SDL_Point rotationCenter;
	Transform(Entity* owner, int width = Globals::TILE_SIZE, int height = Globals::TILE_SIZE, int globalPosX = 0, int globalPosY = 0) : Component(owner) {
		this->globalPosX = (float)globalPosX;
		this->globalPosY = (float)globalPosY;
		isRotated = false;
		rotationAngle = 0;
		rotationCenter.x = 0;
		rotationCenter.y = 0;
		this->width = width;
		this->height = height;
		type = TRANSFORM;
	}
	void rotate(double rotationAngle, SDL_Point* rotationCenter = NULL) {
		isRotated = true;
		this->rotationAngle = rotationAngle;
		if (rotationCenter) {
			this->rotationCenter = *rotationCenter;
		}
	}
	
};

class Input : public Component {
public:
	virtual void update(float deltaTime) = 0;
	Input(Entity* owner) : Component(owner, true) {
		type = INPUT;
	}
};

class Drawable : public Component {
public:
	SDL_Texture* image;
	SDL_Rect* srcRect;
	SDL_Rect startingSrcRect;
	std::string ID;
	Globals::Layers layer;
	SDL_RendererFlip flip;
	bool isRect = false;
	bool fill;
	Drawable(Entity* owner, SDL_Texture* image, std::string ID, Globals::Layers layer, SDL_Rect* srcRect = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) : Component(owner) {
		this->image = image;
		this->ID = ID;
		this->layer = layer;
		this->srcRect = srcRect;
		if(srcRect)
		this->startingSrcRect = *srcRect;
		this->flip = flip;
		type = DRAWABLE;
	}
	Drawable(Entity* owner, Globals::Layers layer, bool fill) : Component(owner) {
		isRect = true;
		this->fill = fill;
		this->layer = layer;
		type = DRAWABLE;
	}
	~Drawable() {
		//SDL_DestroyTexture(image);
		if (srcRect) delete srcRect;
	}

};

class Abilities : public Component {
public:
	virtual void update(float deltaTime) = 0;
	Abilities(Entity* owner) : Component(owner, true) {
		type = ABILITIES;
	}
};

class Stats : public Component {
public:
	int totalHealth;
	int curHealth;
	int mainAttackDmg;
	int secondAttackDmg;
	int speed;
	Stats(Entity* owner) : Component(owner) {
		type = STATS;
	}
};

// For game objects that when interacted - start a dialogue
class Talkable : public Component {
public:
	struct Text {
		std::string text = "";
		std::string speakerID;
		Text(std::string text, std::string speakerID) { this->text = text; this->speakerID = speakerID; };
	};
	struct Dialogue {
		std::vector<Text> dialogue;
		std::string ID;
		Dialogue() {};
		Dialogue(std::vector<Text> dialogue, std::string dialogueID) { this->dialogue = dialogue; ID = dialogueID; };
	};
	std::vector<Dialogue> dialogues;
	
	int lastDialogue = 0;
	Talkable(Entity* owner, std::vector<Dialogue> dialogues) : Component(owner) {
		type = TALKABLE;
		this->dialogues = dialogues;
	}
};