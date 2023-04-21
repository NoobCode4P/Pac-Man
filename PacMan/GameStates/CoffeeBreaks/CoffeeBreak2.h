#pragma once
#include "CoffeeBreak.h"

class CoffeeBreak2 : public CoffeeBreak {
	vector <Sprite> stuckBlinkyPart;
	Sprite* displayStuckPart;
	float timerAutoQuit;
	bool activateAutoQuitTimer;

	void updateSprites();
	void updateAnimation(float deltaTime);
	void loadInitialSprites();
	void checkEndStateConditions();
public:
	CoffeeBreak2(RenderWindow* window);
	~CoffeeBreak2();
	void drawState(RenderWindow* window);
};