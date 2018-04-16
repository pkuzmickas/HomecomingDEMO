#include "NPCSystem.h"

Entity* NPCSystem::createNPC(int posX, int posY, int width, int height, Globals::Layers layer, SDL_Texture * texture, std::string spriteName, Abilities * abilities, Talkable * talkable) {
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
	npc->addComponent(collider);
	CollisionSystem::collidersInScene.push_back(collider);
	Animator* animator = new Animator(npc);
	npc->addComponent(animator);
	AIComponent* ai = new AIComponent(npc);
	npc->addComponent(ai);
	return npc;
}
