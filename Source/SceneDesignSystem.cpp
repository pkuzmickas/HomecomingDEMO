#include "SceneDesignSystem.h"

int SceneDesignSystem::treesSpawned = 0;

Entity* SceneDesignSystem::createTree(int posX, int posY, Globals::Layers layer, SDL_Texture * texture) {
	Entity* tree = new Entity();
	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	Transform* transform = new Transform(tree, width, height, posX, posY);
	tree->addComponent(transform);
	Drawable* drawable = new Drawable(tree, texture, "tree" + treesSpawned, layer, NULL);
	tree->addComponent(drawable);
	Collider* collider = new Collider(tree, Collider::NORMAL, 0, 100, 103, 25);
	tree->addComponent(collider);
	CollisionSystem::collidersInScene.push_back(collider);
	Animator* animator = new Animator(tree);
	tree->addComponent(animator);
	// Leaves fall animations etc.
	/*Animator::Animation walkingd("walking0", { 0, 1, 2, 1 }, walkAnimSpeed, false);
	animator->addAnimation(walkingd);
	Animator::Animation walkingl("walking1", { 3, 4, 5, 4 }, walkAnimSpeed, false);
	animator->addAnimation(walkingl);
	Animator::Animation walkingr("walking2", { 6, 7, 8, 7 }, walkAnimSpeed, false);
	animator->addAnimation(walkingr);
	Animator::Animation walkingu("walking3", { 9, 10, 11, 10 }, walkAnimSpeed, false);
	animator->addAnimation(walkingu);*/

	return tree;
}
