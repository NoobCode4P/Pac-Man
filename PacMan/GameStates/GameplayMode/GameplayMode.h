#pragma once
#include "../GameState.h"
#include "../../GUI/ContextMenu/ContextMenu.h"

class GameplayMode : public GameState{
private:
	Sprite pacmanIcon;

	Text textLevel, textScores;
	Text highscoreText, highscoreNum;
	size_t* bestScores;

	void loadGraphics();
protected:
	Game* game;

	bool GAMEPAUSED, EXITING;

	ContextMenu* pauseMenu, * exitConfirmation;

	//=========Utility methods==========
	void exitConfirming(const Vector2f& mousePosView, const Event& event);
	void updateGame(float deltaTime);
	void updateGraphics();

public:
	GameplayMode(RenderWindow* window, size_t *bestScores);
	~GameplayMode();
	void startMode();
	virtual void updateState(float deltaTime, RenderWindow* window, const Event& event) = 0;
	virtual void drawState(RenderWindow* window);
};