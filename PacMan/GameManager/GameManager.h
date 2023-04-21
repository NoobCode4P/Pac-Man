#pragma once
#include "../GameStatesManager/GameStatesManager.h"

const uint32_t windowWidth = 1000, windowHeight = 720;

class GameManager {
	GameStatesManager* gameStatesManager;
	RenderWindow window;
	Clock clock;
	Image appIcon;

public:
	GameManager();
	~GameManager();
	void startUp();
	void run();
};

