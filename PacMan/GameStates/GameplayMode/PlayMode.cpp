#include "PlayMode.h"

PlayMode::PlayMode(RenderWindow* window, size_t* bestScores) : GameplayMode(window, bestScores) {
	autoQuitTime = 0;
	maxLives = previousNumLives = 5;

	game = new Game(false, &maxLives);

	float originX = Maze::getInstance().getLeftCoordOfTile(Vector2i(1, 34));
	float originY = Maze::getInstance().getTopCoordOfTile(Vector2i(1, 35));

	for (int i = 0; i < maxLives; i++) {
		Sprite livesS;
		livesS.setTexture(*GameTextures::getInstance().getPacManLiveTexture());
		livesS.setOrigin(livesS.getLocalBounds().width / 2, livesS.getLocalBounds().height / 2);
		livesS.setPosition(Vector2f(originX + i * COMMON_SCALE * TILESIZE * 2, originY));
		livesS.setScale(COMMON_SCALE, COMMON_SCALE);

		livesSprites.push_back(livesS);
	}
	
	pauseButton = new TextButton(Maze::getInstance().getCenterOfTile(Vector2i(39, 28)), Vector2f(200, 39), &textFont, "PAUSE", Color(SILVER_COLOR), Color::White, Color(LIME_COLOR), true);

	const vector <string> options = { "RESUME", "QUIT"};
	pauseMenu = new ContextMenu(window, &textFont, options, "GAME PAUSED");

}

PlayMode::~PlayMode() {
	delete pauseButton;
}

void PlayMode::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	if (!game->isGameOver()) {
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
						else if (it->first == "QUIT") {
							EXITING = true;
						}
					}
				});
			}
		}
		else {
			pauseButton->updateButton(mousePosView, event, deltaTime);

			if (pauseButton->isClicked()) {
				game->pauseGame(true);
				game->pauseSoundEffect();
				GAMEPAUSED = true;
			}
			
			updateGame(deltaTime);

			if (livesSprites.size() > maxLives) {
				livesSprites.pop_back();
			}
		}

		updateGraphics();
	}

	else {
		auto updateTimerForAutoQuit = [this,deltaTime]() {
			autoQuitTime += deltaTime;
			if (autoQuitTime >= 5) {
				autoQuitTime = 0;
				QUIT_STATE = true;
			}
		};
		updateTimerForAutoQuit();
	}
}

void PlayMode::drawState(RenderWindow* window) {
	game->draw(window);

	for (const Sprite& liveIcon : livesSprites)
		window->draw(liveIcon);

	pauseButton->drawButton(window);

	GameplayMode::drawState(window);
}