#pragma once
#include "CoffeeBreak1.h"
#include "CoffeeBreak2.h"
#include "CoffeeBreak3.h"

class Cutscenes : public GameState {
	GameStatesManager* manager;

	map <string, Button*> options;
	TextButton* backButton;

public:
	Cutscenes(RenderWindow* window, GameStatesManager* manager);
	~Cutscenes();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	void drawState(RenderWindow* window);
};