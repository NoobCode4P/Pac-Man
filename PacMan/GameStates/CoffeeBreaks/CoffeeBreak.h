#pragma once
#include "../GameState.h"

class CoffeeBreak : public GameState {
protected:
	float leftLimit, rightLimit, centerX;
	float acceleration;

	Sprite pacman, blinky;
	Animation PacmanAnimation, BlinkyAnimation;

	RectangleShape blackRect[2];

	virtual void loadInitialSprites() = 0;
	virtual void checkEndStateConditions() = 0;
	virtual void updateSprites() = 0;
	virtual void updateAnimation(float deltaTime) = 0;

public:
	CoffeeBreak(RenderWindow* window);
	~CoffeeBreak();
	void startMode();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	virtual void drawState(RenderWindow* window);
};