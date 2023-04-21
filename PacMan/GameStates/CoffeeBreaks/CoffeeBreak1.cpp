#include "CoffeeBreak1.h"

CoffeeBreak1::CoffeeBreak1(RenderWindow* window) : CoffeeBreak(window) {
	acceleration = 0;
	LtoR = false;

	loadInitialSprites();
	updateSprites();
}

CoffeeBreak1::~CoffeeBreak1() {}

void CoffeeBreak1::updateSprites() {
	if (!LtoR) {
		PacmanAnimation.setShortClip(GameTextures::getInstance().getLeftPacMan());
		BlinkyAnimation.setShortClip(GameTextures::getInstance().getLeftBlinky());
	}
	else {
		pacman.setScale(2 * COMMON_SCALE, 2 * COMMON_SCALE);
		PacmanAnimation.setShortClip(GameTextures::getInstance().getRightPacMan());
		BlinkyAnimation.setShortClip(GameTextures::getInstance().getBlueFrightenedTextures());
	}
}

void CoffeeBreak1::updateAnimation(float dt) {
	if (!LtoR) {
		if (pacman.getPosition().x >= leftLimit - 100)
			pacman.move(-(dt * 150), 0);

		if (blinky.getPosition().x >= leftLimit - 100)
			blinky.move(-(dt * (150 + (acceleration += dt * 3))), 0);

		else {
			LtoR = true;
			updateSprites();
			acceleration = 0;
		}
	}
	else {
		if (blinky.getPosition().x >= centerX - 80 && pacman.getPosition().x <= rightLimit + 100)
			pacman.move(dt * (150 + (acceleration += dt * 10)), 0);

		if (blinky.getPosition().x <= rightLimit + 100)
			blinky.move(dt * 130, 0);
	}
}

void CoffeeBreak1::loadInitialSprites() {
	pacman.setTexture(GameTextures::getInstance().getLeftPacMan()->operator[](2));
	pacman.setOrigin(pacman.getLocalBounds().width / 2, pacman.getLocalBounds().height / 2);
	pacman.setScale(COMMON_SCALE, COMMON_SCALE);

	blinky.setTexture(GameTextures::getInstance().getLeftBlinky()->operator[](0));
	blinky.setOrigin(blinky.getLocalBounds().width / 2, blinky.getLocalBounds().height / 2);
	blinky.setScale(COMMON_SCALE, COMMON_SCALE);
}

void CoffeeBreak1::checkEndStateConditions() {
	if (LtoR && pacman.getPosition().x >= rightLimit + 100 && blinky.getPosition().x >= rightLimit + 100) {
		if (SoundEffect::getManager().isPlaying(SOUND_COFFEEBREAK))
			SoundEffect::getManager().stopPlayingSound(SOUND_COFFEEBREAK);
		QUIT_STATE = true;
	}
}