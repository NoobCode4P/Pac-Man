#include "CoffeeBreak.h"

CoffeeBreak::CoffeeBreak(RenderWindow* window) : PacmanAnimation(&pacman), BlinkyAnimation(&blinky) {
	leftLimit = (float)window->getSize().x / 4;
	rightLimit = (float)window->getSize().x * 3 / 4;
	centerX = (rightLimit + leftLimit) / 2;

	blackRect[0].setSize(Vector2f(leftLimit, (float)window->getSize().y));
	blackRect[0].setOrigin(blackRect[0].getLocalBounds().width / 2, blackRect[0].getLocalBounds().height / 2);
	blackRect[0].setFillColor(Color::Black);

	blackRect[1] = blackRect[0];
	blackRect[0].setPosition(leftLimit / 2, (float)window->getSize().y / 2);
	blackRect[1].setPosition(rightLimit + leftLimit / 2, (float)window->getSize().y / 2);

	pacman.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(40, 17)).x, (float)window->getSize().y / 2);
	blinky.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(43, 17)).x, (float)window->getSize().y / 2);

	creditText.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 35)).y - 8);
}

CoffeeBreak::~CoffeeBreak() {
	SoundEffect::getManager().stopPlayingSound();
}

void CoffeeBreak::startMode() {
	if (!SoundEffect::getManager().isPlaying(SOUND_COFFEEBREAK))
		SoundEffect::getManager().playSound(SOUND_COFFEEBREAK, true);
}

void CoffeeBreak::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	updateAnimation(deltaTime);

	PacmanAnimation.update(deltaTime);
	BlinkyAnimation.update(deltaTime);
	
	checkEndStateConditions();
}

void CoffeeBreak::drawState(RenderWindow* window) {
	window->draw(pacman);
	window->draw(blinky);

	window->draw(blackRect[0]);
	window->draw(blackRect[1]);

	window->draw(creditText);
}