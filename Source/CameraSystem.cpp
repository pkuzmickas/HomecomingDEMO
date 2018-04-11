#include "CameraSystem.h"

float CameraSystem::posX = 0;
float CameraSystem::posY = 0;
bool CameraSystem::cameraMoving = false;
int CameraSystem::destX = 0;
int CameraSystem::destY = 0;
int CameraSystem::speed = 0;

void CameraSystem::centerAround(int x, int y, int levelWidth, int levelHeight) {
	posX = (float)(x - Globals::SCREEN_WIDTH / 2);
	posY = (float)(y - Globals::SCREEN_HEIGHT / 2);
	if (posX < 0) posX = 0;
	if (posY < 0) posY = 0;
	if (posX + Globals::SCREEN_WIDTH > levelWidth) posX = (float)(levelWidth - Globals::SCREEN_WIDTH);
	if (posY + Globals::SCREEN_HEIGHT> levelHeight) posY = (float)(levelHeight - Globals::SCREEN_HEIGHT);
}

void CameraSystem::update(float deltaTime) {
	if (cameraMoving) {
		if (posX < destX) {
			posX += speed * deltaTime;
		}
		if (posY < destY) {
			posY += speed * deltaTime;
		}
		if (posX >= destX && posY >= destY) {
			posX = (float)destX;
			posY = (float)destY;
			cameraMoving = false;
		}
	}
}

void CameraSystem::moveCamera(int destX, int destY, int speed) {
	CameraSystem::destX = destX;
	CameraSystem::destY = destY;
	CameraSystem::speed = speed;
	cameraMoving = true;
}
