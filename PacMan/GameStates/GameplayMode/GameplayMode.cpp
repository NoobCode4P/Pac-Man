#include "GameplayMode.h"

GameplayMode::GameplayMode(RenderWindow* window, size_t* bestScores) {
	GAMEPAUSED = EXITING = false;
	this->bestScores = bestScores;
	
	exitConfirmation = new ContextMenu(window, &textFont, {"YES", "NO"}, "QUIT GAME?");

	loadGraphics();

	creditText.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(39, 34)));
}

GameplayMode::~GameplayMode() {
	delete game;
	delete pauseMenu;
	delete exitConfirmation;
}

void GameplayMode::loadGraphics() {
	textLevel.setFont(textFont);
	textLevel.setFillColor(Color::Red);
	textLevel.setCharacterSize(38);
	
	textScores.setFont(textFont);
	textScores.setFillColor(Color(TURQUOISE_COLOR));
	textScores.setCharacterSize(25);
	textScores.setStyle(Text::Bold);
	
	highscoreText.setString("HIGH SCORE");
	highscoreText.setFont(textFont);
	highscoreText.setFillColor(Color::Yellow);
	highscoreText.setCharacterSize(18);
	highscoreText.setScale(1.5, 1);
	highscoreText.setLetterSpacing(2);
	highscoreText.setPosition(Vector2f(Maze::getInstance().getLeftCoordOfTile(Vector2i(9, 0)), 1));
	highscoreText.setStyle(Text::Bold);

	highscoreNum.setString(to_string(*bestScores));
	highscoreNum.setFont(textFont);
	highscoreNum.setCharacterSize(18);
	highscoreNum.setFillColor(Color::White);
	highscoreNum.setStyle(Text::Bold);
	highscoreNum.setOrigin(highscoreNum.getLocalBounds().width / 2, highscoreNum.getLocalBounds().height / 2);
	highscoreNum.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(13, 1)));

	pacmanIcon.setTexture(*GameTextures::getInstance().getPacManFigureIconTexture());
	pacmanIcon.setOrigin(pacmanIcon.getLocalBounds().width / 2, pacmanIcon.getLocalBounds().height / 2);
	pacmanIcon.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(39, 9)));

}

void GameplayMode::updateGraphics() {

	textLevel.setString("LEVEL" + to_string(game->getCurrentLevel()));
	textLevel.setOrigin(textLevel.getLocalBounds().width / 2, textLevel.getLocalBounds().height / 2);
	textLevel.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(39, 15)));

	textScores.setString(to_string(game->getCurrentScores()));
	textScores.setOrigin(textScores.getLocalBounds().width / 2, textScores.getLocalBounds().height / 2);
	textScores.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(39, 20)));

	if (game->getCurrentScores() >= *bestScores) {
		*bestScores = game->getCurrentScores();
		highscoreNum.setString(to_string(*bestScores));
		highscoreNum.setOrigin(highscoreNum.getLocalBounds().width / 2, highscoreNum.getLocalBounds().height / 2);
		highscoreNum.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(13, 1)));
	}
}

void GameplayMode::startMode() {
	GameState::startMode();
	game->startGame();
}

void GameplayMode::exitConfirming(const Vector2f& mousePosView, const Event& event) {
	exitConfirmation->update(mousePosView, event);

	mapStructureTraversal<TextButton>(*exitConfirmation->getOptionList(), [this](map <string, TextButton*>::iterator it) {
		if (it->second->isClicked()) {
			if (it->first == "YES") {
				QUIT_STATE = true;
			}
			else if (it->first == "NO") {
				EXITING = false;
			}
		}
	});
}

void GameplayMode::updateGame(float deltaTime) {
	game->processMainGame(deltaTime);
}

void GameplayMode::drawState(RenderWindow* window) {
	window->draw(pacmanIcon);
	window->draw(highscoreText);
	window->draw(highscoreNum);

	window->draw(textLevel);
	window->draw(textScores);

	GameState::drawState(window);

	if (GAMEPAUSED) {
		if (EXITING)
			exitConfirmation->drawOptions(window);
		else
			pauseMenu->drawOptions(window);
	}
}