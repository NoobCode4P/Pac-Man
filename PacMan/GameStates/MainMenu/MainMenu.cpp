#include "MainMenu.h"
#include "../../GameStatesManager/GameStatesManager.h"

MainMenu::MainMenu(RenderWindow* window, GameStatesManager* manager){
	this->GSManager = manager;

	const vector <string> optionsList = {
		"PLAY", "PRACTICE", "LEARN", "EXIT"
	};
	const vector <Color> hoverColors = {
		Color::Green, Color(TURQUOISE_COLOR), Color::Yellow, Color::Red
	};

	pacmanTitle_S.setScale(0.1f, 0.1f);
	pacmanTitle_S.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 4)).y);
	
	int n = static_cast <int> (optionsList.size());
	
	pair <float, float> windowCenter = make_pair((float)window->getSize().x / 2, (float)window->getSize().y / 2 + 10);
	const short characterSize = 25;

	if (n & 1) {
		options[optionsList[n >> 1]] = new TextButton(Vector2f(windowCenter.first, windowCenter.second), Vector2f(160, 50), &textFont, optionsList[n >> 1], Color(SILVER_COLOR), hoverColors[n >> 1], Color::Yellow, true, false, characterSize);
		for (int i = (n >> 1) - 1; i >= 0; i--) {
			options[optionsList[i]] = new TextButton(Vector2f(windowCenter.first, windowCenter.second - 100 * ((n >> 1) - i)), Vector2f(160, 50), &textFont, optionsList[i], Color(SILVER_COLOR), hoverColors[i], Color::Yellow, true, false, characterSize);
			options[optionsList[n - 1 - i]] = new TextButton(Vector2f(windowCenter.first, windowCenter.second + 100 * ((n >> 1) - i)), Vector2f(160, 50), &textFont, optionsList[n - 1 - i], Color(SILVER_COLOR), hoverColors[n - 1 - i], Color::Yellow, true, false, characterSize);
		}
	}
	else {
		for (int i = (n >> 1) - 1; i >= 0; i--) {
			options[optionsList[i]] = new TextButton(Vector2f(windowCenter.first, windowCenter.second - 50 - 100 * ((n >> 1) - 1 - i)), Vector2f(160, 50), &textFont, optionsList[i], Color(SILVER_COLOR), hoverColors[i], Color::Yellow, true, false, characterSize);
			options[optionsList[n - 1 - i]] = new TextButton(Vector2f(windowCenter.first, windowCenter.second + 50 + 100 * ((n - 1 - i) - (n >> 1))), Vector2f(160, 50), &textFont, optionsList[n - 1 - i], Color(SILVER_COLOR), hoverColors[n - 1 - i], Color::Yellow, true, false, characterSize);
		}
	}

	pacmanfigureIcon[0].setTexture(*GameTextures::getInstance().getPacManFigureIconTexture());
	pacmanfigureIcon[0].setOrigin(pacmanfigureIcon[0].getLocalBounds().width / 2, pacmanfigureIcon[0].getLocalBounds().height / 2);
	pacmanfigureIcon[0].setPosition(Maze::getInstance().getCenterOfTile(Vector2i(9, 16)));
	pacmanfigureIcon[0].setScale(2, 2);

	pacmanfigureIcon[1] = pacmanfigureIcon[0];
	pacmanfigureIcon[1].setPosition(Maze::getInstance().getCenterOfTile(Vector2i(40, 16)));
	pacmanfigureIcon[1].setScale(-2, 2);
	

	pacmanEating.setTexture(*GameTextures::getInstance().getPacManEatingTexture());
	pacmanEating.setOrigin(pacmanEating.getLocalBounds().width / 2, pacmanEating.getLocalBounds().height / 2);
	pacmanEating.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 30)).y);

	creditText.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 35)).y - 8);

	coffeeButton = new TextButton(Vector2f((float)window->getSize().x - 50, Maze::getInstance().getCenterOfTile(Vector2i(0, 1)).y), Vector2f(50, 50), &textFont, "C", Color::White, Color(SADDLE_BROWN_COLOR), Color::Yellow, true);

}

MainMenu::~MainMenu() {
	mapStructureTraversal <Button>(options, [](ButtonMapIterator it) { delete it->second;});
	delete coffeeButton;
}

void MainMenu::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	Vector2f mousePosView = window->mapPixelToCoords(Mouse::getPosition(*window));

	mapStructureTraversal <Button> (options, [mousePosView, event, deltaTime](ButtonMapIterator it) { it->second->updateButton(mousePosView, event); });

	for (ButtonMapIterator it = options.begin(); it != options.end(); it++) {
		if (it->second->isClicked()) {
			if (it->first == "PLAY") {
				GSManager->enter(PLAY, window);
			}
			else if (it->first == "PRACTICE") {
				GSManager->enter(PRACTICE, window);
			}
			else if (it->first == "LEARN") {
				GSManager->enter(LEARNGHOSTCHASE, window);
			}
			else if (it->first == "EXIT") {
				QUIT_STATE = true;
				GSManager->endManagement();
			}
			SoundEffect::getManager().playSound(SOUND_CREDIT, false);
			break;
		}
	}
	coffeeButton->updateButton(mousePosView, event);
	if (coffeeButton->isClicked()) {
		GSManager->enter(CUTSCENES, window);
		SoundEffect::getManager().playSound(SOUND_CREDIT, false);
	}
}

void MainMenu::drawState(RenderWindow* window) {
	window->draw(pacmanEating);
	mapStructureTraversal <Button>(options, [window](ButtonMapIterator it) {it->second->drawButton(window); });
	coffeeButton->drawButton(window);

	window->draw(pacmanfigureIcon[0]);
	window->draw(pacmanfigureIcon[1]);
	GameState::drawState(window);
}