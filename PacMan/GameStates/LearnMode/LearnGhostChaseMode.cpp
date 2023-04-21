#include "LearnGhostChaseMode.h"

LearnGhostChaseMode::LearnGhostChaseMode(RenderWindow* window) {	
	pacmanIcon.setTexture(*GameTextures::getInstance().getPacManIconTexture());
	pacmanIcon.setScale(1.5, 1.5);
	pacmanIcon.setOrigin(pacmanIcon.getLocalBounds().width / 2, pacmanIcon.getLocalBounds().height / 2);
	pacmanIcon.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(36, 14)));
	
	ghostIcons.resize(4);
	for (int i = 0; i < 4; i++) {
		ghostIcons[i].setTexture(*GameTextures::getInstance().getGhostIconTextures(i));
		ghostIcons[i].setOrigin(ghostIcons[i].getLocalBounds().width / 2, ghostIcons[i].getLocalBounds().height / 2);
		ghostIcons[i].setPosition(Maze::getInstance().getCenterOfTile(Vector2i(43, 10 + i * 3)));
	}

	PacMan::getInstance().setCurrentLevelData(NULL);
	PacMan::getInstance().setInvincible(true);
	PacMan::getInstance().setVisible(true);
	PacMan::getInstance().revive();
	PacMan::getInstance().setMainTexture();
	PacMan::getInstance().restartAtLocation(Vector2i(16, 23));
	
	ghostGroup["Blinky"] = new Blinky(Vector2i(26, 4), false);;
	ghostGroup["Pinky"] = new Pinky(Vector2i(1, 4), false);
	ghostGroup["Inky"] = new Inky(Vector2i(26, 32), ghostGroup["Blinky"], false);
	ghostGroup["Clyde"] = new Clyde(Vector2i(1, 32), false);

	mapStructureTraversal<Ghost>(ghostGroup, [this](GhostGroupMapIterator it) {
		it->second->ShowPath(true);
		it->second->ShowTarget(true);
		it->second->setVisible(false);
		it->second->setState(CHASE);
		it->second->setCurrentLevelData(NULL);
	});

	ghostButtons["Blinky"] = new ImageButton(Vector2f(Maze::getInstance().getCenterOfTile(Vector2i(6, 1))), Vector2f(40, 40), &ghostGroup["Blinky"]->getTextures(), Color(SILVER_COLOR), Color::White, Color::Red, false, true);
	ghostButtons["Pinky"] = new ImageButton(Vector2f(Maze::getInstance().getCenterOfTile(Vector2i(11, 1))), Vector2f(40, 40), &ghostGroup["Pinky"]->getTextures(), Color(SILVER_COLOR), Color::White, Color::Magenta, false, true);
	ghostButtons["Inky"] = new ImageButton(Vector2f(Maze::getInstance().getCenterOfTile(Vector2i(16, 1))), Vector2f(40, 40), &ghostGroup["Inky"]->getTextures(), Color(SILVER_COLOR), Color::White, Color::Cyan, false, true);
	ghostButtons["Clyde"] = new ImageButton(Vector2f(Maze::getInstance().getCenterOfTile(Vector2i(21, 1))), Vector2f(40, 40), &ghostGroup["Clyde"]->getTextures(), Color(SILVER_COLOR), Color::White, Color(ORANGE_COLOR), false, true);

	backButton = new TextButton(Maze::getInstance().getCenterOfTile(Vector2i(39,28)), Vector2f(200, 40), &textFont, "BACK", Color(SILVER_COLOR), Color::White, Color::Green, true);

	currentDisplayedGhost = "Blinky";
	ghostGroup[currentDisplayedGhost]->setVisible(true);
	ghostButtons[currentDisplayedGhost]->select(true);

	creditText.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(39, 34)));
}

LearnGhostChaseMode::~LearnGhostChaseMode() {
	mapStructureTraversal<Ghost>(ghostGroup, [](GhostGroupMapIterator it) {delete it->second; });
	mapStructureTraversal<Button>(ghostButtons, [](ButtonMapIterator it) {delete it->second; });
	delete backButton;
}

void LearnGhostChaseMode::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	PacMan::getInstance().update(deltaTime);
	
	mapStructureTraversal<Ghost>(ghostGroup, [deltaTime](GhostGroupMapIterator it) {it->second->update(deltaTime); });

	Vector2f mousePosView = window->mapPixelToCoords(Mouse::getPosition(*window));

	mapStructureTraversal<Button>(ghostButtons, [mousePosView, event, deltaTime](ButtonMapIterator it) { it->second->updateButton(mousePosView, event, deltaTime); });

	mapStructureTraversal<Button>(ghostButtons, [this](ButtonMapIterator it) {
		if (it->second->isClicked()) {
			currentDisplayedGhost = it->first;
			ghostGroup[currentDisplayedGhost]->setVisible(true);

			mapStructureTraversal<Button>(ghostButtons, [this](ButtonMapIterator it2) {
				if (it2->first != currentDisplayedGhost) {
					ghostGroup[it2->first]->setVisible(false);
					ghostButtons[it2->first]->select(false);
				}
			});
			if (currentDisplayedGhost == "Inky")
				ghostGroup["Blinky"]->setVisible(true);
		}
	});

	backButton->updateButton(mousePosView, event);

	if (backButton->isClicked())
		QUIT_STATE = true;
}

void LearnGhostChaseMode::drawState(RenderWindow* window) {
	Maze::getInstance().drawMaze(window);

	PacMan::getInstance().draw(window);

	mapStructureTraversal<Ghost>(ghostGroup, [window](GhostGroupMapIterator it) {it->second->drawGhost(window); });

	mapStructureTraversal<Button>(ghostButtons, [window](ButtonMapIterator it) {it->second->drawButton(window); });

	backButton->drawButton(window);

	window->draw(pacmanIcon);
	GameState::drawState(window);

	for (const Sprite& ghostIcon : ghostIcons)
		window->draw(ghostIcon);
}