#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <unordered_set>
#include <map>
using namespace std;
using namespace sf;

#define X_TO_LOAD_SPRITE 230
#define BLINKY_SPRITE_Y 65
#define PINKY_SPRITE_Y 81
#define INKY_SPRITE_Y 97
#define CLYDE_SPRITE_Y 113

#define MAZE_TOPLEFT_COORDS Vector2f(0,60)
#define COMMON_SCALE 2.5f
#define TILESIZE 8.f
#define HALFTILE COMMON_SCALE*TILESIZE/2

#define ORANGE_COLOR 255,165,0
#define SILVER_COLOR 192,192,192
#define HOTPINK_COLOR 255, 105, 180
#define GOLDENROD_COLOR 218,165,32
#define TURQUOISE_COLOR 64,224,208
#define CRIMSON_COLOR 220, 20, 60
#define LIME_COLOR 0,255,0
#define SADDLE_BROWN_COLOR 139,69,19

enum BonusItems {
	PELLET,
	POWER_PELLET,
	CHERRY,
	STRAWBERRY,
	ORANGE,
	APPLE,
	MELON,
	GALAXIAN_FLAGSHIP,
	BELL,
	KEY
};

