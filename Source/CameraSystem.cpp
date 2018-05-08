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
bool CameraSystem::allowedToMove = true;

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
			if (posX + speed * deltaTime + Globals::SCREEN_WIDTH < levelWidth) {
				posX += speed * deltaTime;
			}
			else {
				cameraMoving = false;
				return;
			}
		}
		if (posY < destY) {
			if (posY + speed * deltaTime + Globals::SCREEN_HEIGHT  < levelHeight) {
				posY += speed * deltaTime;
			}
			else {
				cameraMoving = false;
				return;
			}
		}
		if (posX > destX) {
			if (posX - speed * deltaTime > 0) {
				posX -= speed * deltaTime;
			}
			else {
				cameraMoving = false;
				return;
			}
		}
		if (posY > destY) {
			if (posY - speed * deltaTime > 0) {
				posY -= speed * deltaTime;
			}
			else {
				cameraMoving = false;
				return;
			}
		}
		if (abs(posX - destX) <= 10 && abs(posY - destY) <= 10) {
			posX = (float)destX;
			posY = (float)destY;
			cameraMoving = false;
		}
	}
	if (cameraAttached && !cameraMoving) {
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

void CameraSystem::moveAndFollow(int moveToX, int moveToY, float* followPosX, float* followPosY, int speed) {
	follow(followPosX, followPosY);
	moveCamera(moveToX, moveToY, speed);
}
