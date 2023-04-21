#pragma once
#include "../GameState.h"

class MainMenu : public GameState {
	map <string, Button*> options;
	GameStatesManager* GSManager;
	Sprite pacmanfigureIcon[2], pacmanEating;

	Button* coffeeButton;

public:
	MainMenu(RenderWindow* window, GameStatesManager* GSManager);
	~MainMenu();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	void drawState(RenderWindow* window);
};

