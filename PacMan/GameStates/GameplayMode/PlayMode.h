#pragma once
#include "GameplayMode.h"

class PlayMode : public GameplayMode {
	vector <Sprite> livesSprites;
	
	short maxLives, previousNumLives;

	Button* pauseButton;

	float autoQuitTime;
	
public:
	PlayMode(RenderWindow* window, size_t* bestScores);
	~PlayMode();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	void drawState(RenderWindow* window);
};