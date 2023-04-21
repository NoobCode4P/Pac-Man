#pragma once
#include "CoffeeBreak.h"

class CoffeeBreak3 : public CoffeeBreak {
	bool LtoR;

	void updateSprites();
	void updateAnimation(float deltaTime);
	void loadInitialSprites();
	void checkEndStateConditions();
public:
	CoffeeBreak3(RenderWindow* window);
	~CoffeeBreak3();
};