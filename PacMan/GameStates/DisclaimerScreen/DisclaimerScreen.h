#pragma once
#include "../GameState.h"
#include <sstream>

class DisclaimerScreen : public GameState {
	RectangleShape box;
	Text title;
	Text message;
	Sprite mascot;

	TextButton* skipButton;

	void readMessage();
public:
	DisclaimerScreen(RenderWindow* window);
	~DisclaimerScreen();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	void drawState(RenderWindow* window);
};