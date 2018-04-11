#pragma once
#include "Globals.h"
class CameraSystem {
public:
	static float posX;
	static float posY;
	static void centerAround(int x, int y, int levelWidth, int levelHeight);
	static void update(float deltaTime);
	static bool isCameraMoving() { return cameraMoving; }
	static void moveCamera(int destX, int destY, int speed);
private:
	CameraSystem();
	static bool cameraMoving;
	static int destX;
	static int destY;
	static int speed;
};