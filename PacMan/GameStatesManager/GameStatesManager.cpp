#include "GameStatesManager.h"

GameStatesManager::GameStatesManager(){
	bestScores = 0;
	QUITGAME = false;
}

GameStatesManager::~GameStatesManager() {
	while (!gameStates.empty())
		quitCurrentState();
}

void GameStatesManager::start(RenderWindow* window) {
	gameStates.push(new DisclaimerScreen(window));
}

void GameStatesManager::quitCurrentState() {
	delete gameStates.top();
	gameStates.pop();
}

void GameStatesManager::enter(GAMESTATES newState, RenderWindow* window) {
	switch (newState) {
	case MAINMENU:
		gameStates.push(new MainMenu(window, this));
		break;
	case PLAY:
		gameStates.push(new PlayMode(window, &bestScores));
		break;
	case PRACTICE:
		gameStates.push(new PracticeMode(window));
		break;
	case LEARNGHOSTCHASE:
		gameStates.push(new LearnGhostChaseMode(window));
		break;
	case CUTSCENES:
		gameStates.push(new Cutscenes(window, this));
		break;
	case COFFEEBREAK1:
		gameStates.push(new CoffeeBreak1(window));
		break;
	case COFFEEBREAK2:
		gameStates.push(new CoffeeBreak2(window));
		break;
	case COFFEEBREAK3:
		gameStates.push(new CoffeeBreak3(window));
		break;
	}
	gameStates.top()->startMode();
}

void GameStatesManager::endManagement() {
	QUITGAME = true;
}

bool GameStatesManager::isDone() const {
	return QUITGAME;
}

void GameStatesManager::update(float deltaTime, RenderWindow* window, const Event& event) {
	if (!gameStates.empty()) {
		GameState* currentGameState = gameStates.top();
		currentGameState->updateState(deltaTime, window, event);

		if (currentGameState->isStateEnded()) {
			quitCurrentState();
			if (gameStates.empty()) {
				if (!QUITGAME)
					enter(MAINMENU, window);
			}
		}
	}
}

void GameStatesManager::draw(RenderWindow* window) {
	gameStates.top()->drawState(window);
}
