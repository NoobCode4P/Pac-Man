#include "Cutscenes.h"
#include "../../GameStatesManager/GameStatesManager.h"

Cutscenes::Cutscenes(RenderWindow* window, GameStatesManager* m) {
	manager = m;

	const float middleX = (float)window->getSize().x / 2;
	const float startY = Maze::getInstance().getCenterOfTile(Vector2i(0, 10)).y;
	const Color activeColor = Color::White, hoverColor = Color::Green, selectedColor = Color::Yellow;
	const Vector2f buttonSize(160, 50);

	for (int i = 0; i < 3; i++) {
		string name = "Coffee Break " + to_string(i + 1);
		options[name] = new TextButton(Vector2f(middleX, startY + i * 100), Vector2f(160, 50), &textFont, name, activeColor, hoverColor, selectedColor, true);
	}

	backButton = new TextButton(Vector2f(middleX, startY + 4 * 100), Vector2f(80, 50), &textFont, "BACK", activeColor, hoverColor, selectedColor, true);

	pacmanTitle_S.setScale(0.1f, 0.1f);
	pacmanTitle_S.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 4)).y);
	
	creditText.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 35)).y - 8);

}

Cutscenes::~Cutscenes() {
	mapStructureTraversal<Button>(options, [](ButtonMapIterator it) {delete it->second; });
	delete backButton;
}

void Cutscenes::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	Vector2f mousePosView = window->mapPixelToCoords(Mouse::getPosition(*window));

	mapStructureTraversal <Button>(options, [mousePosView, event, deltaTime](ButtonMapIterator it) { it->second->updateButton(mousePosView, event); });

	for (ButtonMapIterator it = options.begin(); it != options.end(); it++) {
		if (it->second->isClicked()) {
			if (it->first == "Coffee Break 1") {
				manager->enter(COFFEEBREAK1, window);
			}
			else if (it->first == "Coffee Break 2") {
				manager->enter(COFFEEBREAK2, window);
			}
			else if (it->first == "Coffee Break 3") {
				manager->enter(COFFEEBREAK3, window);
			}
			SoundEffect::getManager().playSound(SOUND_CREDIT, false);
			break;
		}
	}
	backButton->updateButton(mousePosView, event);

	if (backButton->isClicked()) {
		QUIT_STATE = true;
	}
}

void Cutscenes::drawState(RenderWindow* window) {
	mapStructureTraversal <Button>(options, [window](ButtonMapIterator it) {it->second->drawButton(window); });
	backButton->drawButton(window);
	GameState::drawState(window);
}