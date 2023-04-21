#include "CoffeeBreak2.h"

CoffeeBreak2::CoffeeBreak2(RenderWindow* window) : CoffeeBreak(window) {
	timerAutoQuit = 0;
	activateAutoQuitTimer = false;

	const vector <Texture>* ptr = GameTextures::getInstance().getStuckBlinkyPartsTextures();
	stuckBlinkyPart.resize(ptr->size());
	for (int i = 0; i < stuckBlinkyPart.size(); i++) {
		stuckBlinkyPart[i].setTexture(ptr->operator[](i));
		stuckBlinkyPart[i].setOrigin(stuckBlinkyPart[i].getLocalBounds().width, stuckBlinkyPart[i].getLocalBounds().height / 2);
		stuckBlinkyPart[i].setScale(COMMON_SCALE, COMMON_SCALE);
		stuckBlinkyPart[i].setPosition((float)window->getSize().x / 2, (float)window->getSize().y / 2);
	}
	displayStuckPart = &stuckBlinkyPart[0];

	loadInitialSprites();
	updateSprites();
}

CoffeeBreak2::~CoffeeBreak2(){}

void CoffeeBreak2::drawState(RenderWindow* window) {
	window->draw(*displayStuckPart);
	CoffeeBreak::drawState(window);
}

void CoffeeBreak2::updateSprites() {
	PacmanAnimation.setShortClip(GameTextures::getInstance().getLeftPacMan());
	BlinkyAnimation.setShortClip(GameTextures::getInstance().getLeftBlinky());
}

void CoffeeBreak2::updateAnimation(float dt) {
	float xToStop = centerX - 47;
	float slowDelta = centerX - xToStop;

	if (pacman.getPosition().x >= leftLimit - 100)
		pacman.move(-(dt * 150), 0);

	if (blinky.getPosition().x >= xToStop) {
		if (pacman.getPosition().x < centerX) {
			if (blinky.getPosition().x <= centerX) {
				blinky.move(-(dt * (150 + (acceleration -= dt * 200))), 0);

				if (blinky.getPosition().x >= xToStop + slowDelta * 3 / 4)
					displayStuckPart = &stuckBlinkyPart[1];
				else if (blinky.getPosition().x >= xToStop + slowDelta / 2)
					displayStuckPart = &stuckBlinkyPart[2];
				else if (blinky.getPosition().x >= xToStop + slowDelta / 4)
					displayStuckPart = &stuckBlinkyPart[3];

				if (blinky.getPosition().x < xToStop) {
					BlinkyAnimation.setSwitchTime(2);
					blinky.setTexture(GameTextures::getInstance().getWoundedBlinkyTextures()->operator[](0));
					BlinkyAnimation.setShortClip(GameTextures::getInstance().getWoundedBlinkyTextures());
					displayStuckPart = &stuckBlinkyPart[4];
					activateAutoQuitTimer = true;
				}

			}
			else
				blinky.move(-dt * 150, 0);
		}
	}
	else {
		if (activateAutoQuitTimer)
			timerAutoQuit += dt;
	}
}

void CoffeeBreak2::loadInitialSprites() {
	pacman.setTexture(GameTextures::getInstance().getLeftPacMan()->operator[](2));
	pacman.setOrigin(pacman.getLocalBounds().width / 2, pacman.getLocalBounds().height / 2);
	pacman.setScale(COMMON_SCALE, COMMON_SCALE);

	blinky.setTexture(GameTextures::getInstance().getLeftBlinky()->operator[](0));
	blinky.setOrigin(blinky.getLocalBounds().width / 2, blinky.getLocalBounds().height / 2);
	blinky.setScale(COMMON_SCALE, COMMON_SCALE);
}

void CoffeeBreak2::checkEndStateConditions() {
	if (timerAutoQuit >= 5.5 && pacman.getPosition().x < leftLimit - 100) {
		if (SoundEffect::getManager().isPlaying(SOUND_COFFEEBREAK))
			SoundEffect::getManager().stopPlayingSound(SOUND_COFFEEBREAK);
		QUIT_STATE = true;
	}
}