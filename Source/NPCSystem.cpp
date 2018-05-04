#include "NPCSystem.h"

Entity* NPCSystem::createNPC(int posX, int posY, int width, int height, Globals::Layers layer, SDL_Texture * texture, std::string spriteName, std::string* speakerID) {
	Entity* npc = new Entity();
	Transform* transform = new Transform(npc, width, height, posX, posY);
	npc->addComponent(transform);
	SDL_Rect* srcRect = new SDL_Rect();
	srcRect->h = height;
	srcRect->w = width;
	srcRect->x = 1 * srcRect->w;
	srcRect->y = Animator::LookDirection::RIGHT * srcRect->h;
	Drawable* drawable = new Drawable(npc, texture, spriteName, layer, srcRect);
	npc->addComponent(drawable);
	Movement* movement = new Movement(npc);
	npc->addComponent(movement);
	Collider* collider = new Collider(npc, Collider::NORMAL);
	collider->colBox.h -= 20;
	npc->addComponent(collider);
	CollisionSystem::collidersInScene.push_back(collider);
	Animator* animator = new Animator(npc);
	npc->addComponent(animator);
	int walkAnimSpeed = 200;
	Animator::Animation walkingd("walking0", { 0, 1, 2, 1 }, walkAnimSpeed, false);
	animator->addAnimation(walkingd);
	Animator::Animation walkingl("walking1", { 3, 4, 5, 4 }, walkAnimSpeed, false);
	animator->addAnimation(walkingl);
	Animator::Animation walkingr("walking2", { 6, 7, 8, 7 }, walkAnimSpeed, false);
	animator->addAnimation(walkingr);
	Animator::Animation walkingu("walking3", { 9, 10, 11, 10 }, walkAnimSpeed, false);
	animator->addAnimation(walkingu);
	
	
	// TODO DIALOGUE IMPLEMENTATION FOR STATIC AI
	/*if (speakerID) { 
		Talkable* talkable = new Talkable(,);
		npc->addComponent(talkable);
	}*/
	return npc;
}

Entity * NPCSystem::createSoldier(int posX, int posY, int width, int height, Globals::Layers layer, SDL_Texture * texture, std::string spriteName, std::string * speakerID) {
	Entity* soldier = createNPC(posX, posY, width, height, layer, texture, spriteName, speakerID);
	Stats* stats = new Stats(soldier);
	soldier->addComponent(stats);
	stats->totalHealth = 300;
	stats->curHealth = 300;
	AISoldier* ai = new AISoldier(soldier);
	soldier->addComponent(ai);
	return soldier;
}
