#include "Blinky.h"

Blinky::Blinky(Vector2i gridPos, bool applyStatesInterchange) : Ghost(gridPos, applyStatesInterchange){
	loadDirectionalTexture();
	
	if (!Maze::getInstance().isInHouse(gridPos)) {
		mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
		isSpawned = isOutside = true;
	}
	else
		moveToOriginalStartingPosition();

	pathSketcher->setColor(Color(CRIMSON_COLOR));
	targetTile.setFillColor(Color::Red);
}

Blinky::Blinky() : Ghost(Vector2i(13,14)){
	loadDirectionalTexture();
	moveToOriginalStartingPosition();
	pathSketcher->setColor(Color(CRIMSON_COLOR));
	targetTile.setFillColor(Color::Red);
}

Blinky::~Blinky(){}

void Blinky::loadDirectionalTexture() {
	upTexture = GameTextures::getInstance().getUpBlinky();
	leftTexture = GameTextures::getInstance().getLeftBlinky();
	downTexture = GameTextures::getInstance().getDownBlinky();
	rightTexture = GameTextures::getInstance().getRightBlinky();

	mainSprite.setTexture(leftTexture->operator[](0));
	mainSprite.setOrigin(mainSprite.getLocalBounds().width / 2, mainSprite.getLocalBounds().height / 2);
	mainSprite.setScale(COMMON_SCALE, COMMON_SCALE);
}

Vector2i Blinky::getScatterTargetPosition() const {
	return Vector2i(25, 0);
}

Vector2i Blinky::getChaseTargetPosition() const {
	return PacMan::getInstance().getGridPos();
}

Vector2i Blinky::getRevivePosition() const {
	return Vector2i(13, 17);
}

void Blinky::moveToOriginalStartingPosition() {
	gridPos = Vector2i(13, 14);
	currentDirection = NONE;
	mainSprite.setTexture(leftTexture->operator[](0));
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
	mainSprite.move(HALFTILE, 0);
	isSpawned = isOutside = true;
}