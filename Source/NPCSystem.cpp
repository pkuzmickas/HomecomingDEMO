#include "NPCSystem.h"

Entity* NPCSystem::createNPC(int posX, int posY, Globals::Layers layer, SDL_Texture * texture, std::string spriteName, Abilities * abilities, Talkable * talkable) {
	Entity* npc = new Entity();
	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	Transform* transform = new Transform(npc, width, height, posX, posY);
	npc->addComponent(transform);
	Drawable* drawable = new Drawable(npc, texture, spriteName, layer, NULL);
	npc->addComponent(drawable);
	Collider* collider = new Collider(npc, Collider::NORMAL);
	npc->addComponent(collider);
	CollisionSystem::collidersInScene.push_back(collider);
	Animator* animator = new Animator(npc);
	npc->addComponent(animator);
	AIComponent* ai = new AIComponent(npc);
	npc->addComponent(ai);
	return npc;
}
