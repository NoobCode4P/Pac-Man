#include "Clyde.h"

Clyde::Clyde(Vector2i gridPos, bool applyStatesInterchange) : Ghost(gridPos, applyStatesInterchange) {
	loadDirectionalTexture();
	
	if (!Maze::getInstance().isInHouse(gridPos)) {
		mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
		isSpawned = isOutside = true;
	}
	else
		moveToOriginalStartingPosition();

	pathSketcher->setColor(Color(GOLDENROD_COLOR));
	targetTile.setFillColor(Color(ORANGE_COLOR));
}

Clyde::Clyde() : Ghost(Vector2i(16,17)){
	loadDirectionalTexture();
	moveToOriginalStartingPosition();
	pathSketcher->setColor(Color(GOLDENROD_COLOR));
	targetTile.setFillColor(Color(ORANGE_COLOR));
}

Clyde::~Clyde() {}

void Clyde::loadDirectionalTexture() {
	upTexture = GameTextures::getInstance().getUpClyde();
	leftTexture = GameTextures::getInstance().getLeftClyde();
	downTexture = GameTextures::getInstance().getDownClyde();
	rightTexture = GameTextures::getInstance().getRightClyde();

	mainSprite.setTexture(upTexture->operator[](0));
	mainSprite.setOrigin(mainSprite.getLocalBounds().width / 2, mainSprite.getLocalBounds().height / 2);
	mainSprite.setScale(COMMON_SCALE, COMMON_SCALE);
}

void Clyde::drawTargetingSystem(RenderWindow* window) {
	if (getState() == CHASE && isSpawned) {
		CircleShape circle(8 * TILESIZE * COMMON_SCALE);
		circle.setOrigin(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2);
		circle.setPosition(PacMan::getInstance().getPosition());
		circle.setFillColor(Color::Transparent);
		circle.setOutlineColor(Color(GOLDENROD_COLOR));
		circle.setOutlineThickness(2);
		window->draw(circle);
	}

	Ghost::drawTargetingSystem(window);
}

Vector2i Clyde::getScatterTargetPosition() const {
	return Vector2i(0, NUMBER_OF_ROWS - 1);
}

Vector2i Clyde::getChaseTargetPosition() const {
	auto distance = [](const Vector2i& a, const Vector2i& b) -> double{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	};

	if (distance(gridPos, PacMan::getInstance().getGridPos()) > 8)
		return PacMan::getInstance().getGridPos();

	return getScatterTargetPosition();
}

Vector2i Clyde::getRevivePosition() const {
	return Vector2i(16, 17);
}

void Clyde::moveOutOfTheHouse(float delta) {
	float middleVerticalLine = Maze::getInstance().getLeftCoordOfTile(Vector2i(14, 17));
	if (mainSprite.getPosition().x - delta > middleVerticalLine) {
		currentDirection = LEFT;
		mainSprite.move(-delta, 0);
	}
	else
		Ghost::moveOutOfTheHouse(delta);
}

void Clyde::moveIntoTheHouse(float delta) {
	float lowerLimit = Maze::getInstance().getCenterOfTile(Vector2i(13, 17)).y;
	if (mainSprite.getPosition().y + delta < lowerLimit) {
		currentDirection = DOWN;
		mainSprite.move(0, delta);
	}
	else {
		float limitLine = Maze::getInstance().getLeftCoordOfTile(Vector2i(16, 17)) - 1;
		if (mainSprite.getPosition().x + delta < limitLine) {
			currentDirection = RIGHT;
			mainSprite.move(delta, 0);
		}
		else {
			isOutside = false;
			isSpawned = false;
			gridPos = getRevivePosition();
			currentDirection = UP;
			activateDotCounter = true;
		}
	}
}

void Clyde::moveToOriginalStartingPosition() {
	gridPos = Vector2i(16, 17);
	currentDirection = UP;
	mainSprite.setTexture(upTexture->operator[](0));
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
	mainSprite.move(-HALFTILE + 1, 0);
	isSpawned = isOutside = false;
}