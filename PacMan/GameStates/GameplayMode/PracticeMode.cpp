#include "PracticeMode.h"

PracticeMode::PracticeMode(RenderWindow* window) : GameplayMode(window, &bestPracticeScore) {
	game = new Game();

	const vector <string> options = {
		"SHOW PATHS", "SHOW TARGETS", "INVINCIBLE", "PAUSE"
	};
	
	const vector <Vector2i> coords = {
		Vector2i(34,25), Vector2i(44, 25),
		Vector2i(34,30), Vector2i(44, 30)
	};

	for (int i = 0; i < options.size(); i++) {
		buttons[options[i]] = new TextButton(Maze::getInstance().getCenterOfTile(coords[i]), Vector2f(150, 50), &textFont, options[i], Color(SILVER_COLOR), Color::White, Color(LIME_COLOR), i == options.size() - 1);
	}
	const vector <string> pauseOptions = {
		"RESUME", "RESTART LEVEL", "SKIP LEVEL", "QUIT"
	};
	pauseMenu = new ContextMenu(window, &textFont, pauseOptions, "GAME PAUSED");

}

PracticeMode::~PracticeMode() {
	mapStructureTraversal<Button>(buttons, [](ButtonMapIterator it) {delete it->second; });
}

void PracticeMode::drawState(RenderWindow* window) {
	game->draw(window);

	mapStructureTraversal<Button>(buttons, [window](ButtonMapIterator it) {it->second->drawButton(window); });

	GameplayMode::drawState(window);
}

void PracticeMode::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	Vector2f mousePosView = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (GAMEPAUSED) {
		if (EXITING) {
			exitConfirming(mousePosView, event);
		}
		else {
			pauseMenu->update(mousePosView, event);

			mapStructureTraversal<TextButton>(*pauseMenu->getOptionList(), [this](map <string, TextButton*>::iterator it) {
				if (it->second->isClicked()) {
					if (it->first == "RESUME") {
						game->pauseGame(false);
						game->pauseSoundEffect(false);
						GAMEPAUSED = false;
					}
					else if (it->first == "RESTART LEVEL") {
						game->restartLevel();
						game->pauseGame(false);
						GAMEPAUSED = false;
					}
					else if (it->first == "SKIP LEVEL") {
						game->skipLevel();
						game->pauseGame(false);
						GAMEPAUSED = false;
					}
					else if (it->first == "QUIT") {
						EXITING = true;
					}
				}
			});
		}

	}

	else {
		mapStructureTraversal<Button>(buttons, [mousePosView, event, deltaTime](ButtonMapIterator it) { it->second->updateButton(mousePosView, event, deltaTime); });

		mapStructureTraversal<Button>(buttons, [this](ButtonMapIterator it) {
			if (it->second->isClicked()) {
				if (it->first == "INVINCIBLE") {

					game->setPacManInvincible(it->second->isSelected());
				}
				else if (it->first == "SHOW PATHS") {
					game->setShowPath(it->second->isSelected());
				}
				else if (it->first == "SHOW TARGETS") {
					game->setShowTarget(it->second->isSelected());
				}
				else if (it->first == "PAUSE") {
					game->pauseGame(true);
					game->pauseSoundEffect();
					GAMEPAUSED = true;
				}
			}
		});

		updateGame(deltaTime);
	}

	updateGraphics();
}