#pragma once
#include "GameplayMode.h"

class PracticeMode : public GameplayMode {
	
	map <string, Button*> buttons;

	size_t bestPracticeScore = 0;

public:
	PracticeMode(RenderWindow* window);
	~PracticeMode();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	void drawState(RenderWindow* window);
};