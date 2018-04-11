#pragma once
#include <vector>
#include <SDL.h>
/*
Class for holding the constant global values for the game
The names should be self explanatory and if not, should be changed to be so
*/

#define SIN45 0.707f
#define ASSET_DIR "Assets/"
#define CHARACTER_DIR "Characters/"
#define LEVEL_DESIGN_DIR "LevelDesign/"
#define LEVEL_DIR "Levels/"

class Globals {

public:

	static const int SCREEN_WIDTH = 1000;
	static const int SCREEN_HEIGHT = 800;
	static const int TILE_SIZE = 32;
	static const int ANIMSPEED = 250;
	static const int COL_UP_ERROR = 5;
	static const int COL_DOWN_ERROR = 5;
	static const int COL_LEFT_ERROR = 5;
	static const int COL_RIGHT_ERROR = 5;
	static const int MAX_LAYERS = 5;
	static const int ATTACK_DIST = 15;
	static const int DASHING_DIST = 400;
	static const int SLASH_SPEED = 25; //the lower the faster
	static const int KUNAI_ANIM_SPEED = 25; //the lower the faster
											//static const int KUNAI_SPEED = 1; //the higher the faster
	static const int INDICATOR_WIDTH = 32;
	static const int INDICATOR_HEIGHT = 160;
	static const int SLOW_MOTION_SPEED = 25;
	static const int DEFAULT_PLAYER_SPEED = 200;
	static const int CONSOLE_FONT_SIZE = 28; // 28
	static const int DIALOGUE_LETTER_ANIM_TIME = 80; // milliseconds
	static const int DIALOGUE_LETTER_ANIM_TIME_FAST = 5; // milliseconds

	static const int KUNAI_SPEED = 1;
	static const int MAIN_ATT_CD = 0;
	static const int SECOND_ATT_CD = 0;
	static const int SPECIAL_ATT_CD = 2;
	static const int DASH_MOVE_CD = 0;
	
	enum Layers {
		BACKGROUND0,
		BACKGROUND1,
		PLAYER,
		FOREGROUND,
		UI,
		END_MARKER
	};

	static std::vector<SDL_Event>& GetFrameEvents() {
		static std::vector<SDL_Event> frame_events;
		return frame_events;
	}

};

