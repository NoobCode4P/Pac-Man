#include "GameManager.h"

GameManager::GameManager() {
	window.create(VideoMode(windowWidth, windowHeight), "PACMAN GAME", Style::Close);

	gameStatesManager = new GameStatesManager();

	if (appIcon.loadFromFile("Resources/Textures/pac_man_icon.png")) {
		window.setIcon(appIcon.getSize().x, appIcon.getSize().y, appIcon.getPixelsPtr());
	}
}

GameManager::~GameManager() {
	delete gameStatesManager;
}

void GameManager::startUp() {
	gameStatesManager->start(&window);
}

void GameManager::run() {
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		
		gameStatesManager->update(deltaTime, &window, event);

		if (gameStatesManager->isDone()) {
			window.close();
			break;
		}

		window.clear();

		gameStatesManager->draw(&window);

		window.display();
	}
}