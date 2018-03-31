#include "Camera.h"

float Camera::posX = 0;
float Camera::posY = 0;


void Camera::centerAround(int x, int y, int levelWidth, int levelHeight) {
	posX = x - Globals::SCREEN_WIDTH / 2;
	posY = y - Globals::SCREEN_HEIGHT / 2;
	if (posX < 0) posX = 0;
	if (posY < 0) posY = 0;
	if (posX > levelWidth) posX = levelWidth - Globals::SCREEN_WIDTH;
	if (posY > levelHeight) posY = levelHeight - Globals::SCREEN_HEIGHT;
}
