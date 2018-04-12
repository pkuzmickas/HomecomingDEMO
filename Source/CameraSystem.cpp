#include "CameraSystem.h"

float CameraSystem::posX = 0;
float CameraSystem::posY = 0;
float* CameraSystem::followX = NULL;
float* CameraSystem::followY = NULL;
bool CameraSystem::cameraMoving = false;
bool CameraSystem::cameraAttached = false;
int CameraSystem::destX = 0;
int CameraSystem::destY = 0;
int CameraSystem::speed = 0;
int CameraSystem::levelHeight = 0;
int CameraSystem::levelWidth = 0;

void CameraSystem::setUp(int levelWidth, int levelHeight) {
	CameraSystem::levelWidth = levelWidth;
	CameraSystem::levelHeight = levelHeight;
}

void CameraSystem::centerAround(int x, int y) {
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
	if (cameraAttached) {
		centerAround(*followX, *followY);
	}
}

void CameraSystem::moveCamera(int destX, int destY, int speed) {
	CameraSystem::destX = destX;
	CameraSystem::destY = destY;
	CameraSystem::speed = speed;
	cameraMoving = true;
}

void CameraSystem::follow(float * posX, float * posY) {
	cameraAttached = true;
	followX = posX;
	followY = posY;
}
