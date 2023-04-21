#include "CoffeeBreak3.h"

CoffeeBreak3::CoffeeBreak3(RenderWindow* window) : CoffeeBreak(window) {
	acceleration = 0;
	LtoR = false;

	loadInitialSprites();
	updateSprites();
}

CoffeeBreak3::~CoffeeBreak3() {}

void CoffeeBreak3::loadInitialSprites() {
	pacman.setTexture(GameTextures::getInstance().getLeftPacMan()->operator[](2));
	pacman.setOrigin(pacman.getLocalBounds().width / 2, pacman.getLocalBounds().height / 2);
	pacman.setScale(COMMON_SCALE, COMMON_SCALE);

	blinky.setTexture(GameTextures::getInstance().getPatchedUpBlinkyTextures()->operator[](0));
	blinky.setOrigin(blinky.getLocalBounds().width / 2, blinky.getLocalBounds().height / 2);
	blinky.setScale(COMMON_SCALE, COMMON_SCALE);
}

void CoffeeBreak3::updateSprites() {
	if (!LtoR) {
		PacmanAnimation.setShortClip(GameTextures::getInstance().getLeftPacMan());
		BlinkyAnimation.setShortClip(GameTextures::getInstance().getPatchedUpBlinkyTextures());
	}
	else {
		blinky.setTextureRect(IntRect(0,0,22,14));
		blinky.setTexture(GameTextures::getInstance().getBlinkyTrueFormTextures()->operator[](0));
		blinky.setOrigin(blinky.getLocalBounds().width / 2, blinky.getLocalBounds().height / 2);
		BlinkyAnimation.setShortClip(GameTextures::getInstance().getBlinkyTrueFormTextures());
	}
}

void CoffeeBreak3::updateAnimation(float dt) {
	if (!LtoR) {
		if (pacman.getPosition().x >= leftLimit - 100)
			pacman.move(-(dt * 140), 0);

		if (blinky.getPosition().x >= leftLimit - 100)
			blinky.move(-(dt * 140), 0);

		else {
			LtoR = true;
			updateSprites();
		}
	}
	else {
		if (blinky.getPosition().x <= rightLimit + 100)
			blinky.move(dt * 140, 0);
	}
}

void CoffeeBreak3::checkEndStateConditions() {
	if (LtoR && pacman.getPosition().x < leftLimit + 100 && blinky.getPosition().x >= rightLimit + 100) {
		if (SoundEffect::getManager().isPlaying(SOUND_COFFEEBREAK))
			SoundEffect::getManager().stopPlayingSound(SOUND_COFFEEBREAK);
		QUIT_STATE = true;
	}
}