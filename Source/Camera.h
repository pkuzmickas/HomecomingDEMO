#pragma once
#include "Globals.h"
class Camera {
public:
	static float posX;
	static float posY;
	static void centerAround(int x, int y, int levelWidth, int levelHeight);
private:
	Camera();
};