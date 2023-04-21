#pragma once
#include "../GameStates/MainMenu/MainMenu.h"
#include "../GameStates/LearnMode/LearnGhostChaseMode.h"
#include "../GameStates/GameplayMode/PlayMode.h"
#include "../GameStates/GameplayMode/PracticeMode.h"
#include "../GameStates/DisclaimerScreen/DisclaimerScreen.h"
#include "../GameStates/CoffeeBreaks/Cutscenes.h"

class GameStatesManager{
	stack <GameState*> gameStates;
	size_t bestScores;
	bool QUITGAME;
public:
	GameStatesManager();
	~GameStatesManager();
	void start(RenderWindow* window);
	void update(float deltaTime, RenderWindow* window, const Event& event);
	void draw(RenderWindow* window);
	void enter(GAMESTATES newState, RenderWindow* window);
	void quitCurrentState();
	void endManagement();
	bool isDone() const;
};
