#pragma once
#include "Globals.h"
class CameraSystem {
public:
	static float posX;
	static float posY;
	static void setUp(int levelWidth, int levelHeight);
	static void centerAround(int x, int y);
	static void update(float deltaTime);
	static bool isCameraMoving() { return cameraMoving; }
	static bool isCameraFollowing() { return cameraAttached; }
	static void moveCamera(int destX, int destY, int speed);
	static void follow(float* posX, float* posY);
	static void moveAndFollow(int moveToX, int moveToY, float* followPosX, float* followPosY, int speed);
	static void detachCamera() { cameraAttached = false; }
	static bool allowedToMove;
private:
	CameraSystem();
	static bool cameraMoving;
	static int destX;
	static int destY;
	static int speed;
	static int levelWidth;
	static int levelHeight;
	static float* followX;
	static float* followY;
	static bool cameraAttached;
};