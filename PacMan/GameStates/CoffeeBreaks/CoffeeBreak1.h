#pragma once
#include "CoffeeBreak.h"

class CoffeeBreak1 : public CoffeeBreak {
	bool LtoR;
	
	void updateSprites();
	void updateAnimation(float deltaTime);
	void loadInitialSprites();
	void checkEndStateConditions();
public:
	CoffeeBreak1(RenderWindow* window);
	~CoffeeBreak1();
};