#include "Pinky.h"

Pinky::Pinky(Vector2i gridPos, bool applyStatesInterchange) : Ghost(gridPos, applyStatesInterchange) {
	loadDirectionalTexture();

	if (!Maze::getInstance().isInHouse(gridPos)){
		mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
		isSpawned = isOutside = true;
	}
	else
		moveToOriginalStartingPosition();

	pathSketcher->setColor(Color(HOTPINK_COLOR));
	targetTile.setFillColor(Color::Magenta);
}

Pinky::Pinky() : Ghost(Vector2i(13, 17)) {
	loadDirectionalTexture();
	moveToOriginalStartingPosition();
	pathSketcher->setColor(Color(HOTPINK_COLOR));
	targetTile.setFillColor(Color::Magenta);
}

Pinky::~Pinky(){}

void Pinky::loadDirectionalTexture() {
	upTexture = GameTextures::getInstance().getUpPinky();
	leftTexture = GameTextures::getInstance().getLeftPinky();
	downTexture = GameTextures::getInstance().getDownPinky();
	rightTexture = GameTextures::getInstance().getRightPinky();

	mainSprite.setTexture(downTexture->operator[](0));
	mainSprite.setOrigin(mainSprite.getLocalBounds().width / 2, mainSprite.getLocalBounds().height / 2);
	mainSprite.setScale(COMMON_SCALE, COMMON_SCALE);
}

void Pinky::drawTargetingSystem(RenderWindow* window) {
	if (getState() == CHASE && isSpawned) {
		Vector2i target = PacMan::getInstance().getGridPos();
		Vector2f pacmanPos = PacMan::getInstance().getPosition();

		vector <RectangleShape> lines;
		
		switch (PacMan::getInstance().getCurrentDirection()) {
		case UP: {
			target.y -= 4;
			float length = pacmanPos.y - Maze::getInstance().getCenterOfTile(target).y;
			float length2 = Maze::getInstance().getCenterOfTile(target).x - Maze::getInstance().getCenterOfTile(Vector2i(target.x - 4, target.y)).x;
			target.x -= 4;
			RectangleShape vline(Vector2f(2, length)), hline(Vector2f(length2, 2));
			vline.setOrigin(vline.getLocalBounds().width / 2, vline.getLocalBounds().height);
			vline.setPosition(pacmanPos);

			hline.setOrigin(0, hline.getLocalBounds().height / 2);
			hline.setPosition(Maze::getInstance().getCenterOfTile(target));

			vline.setFillColor(Color(SILVER_COLOR));
			hline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(vline);
			lines.push_back(hline);
			
			break;
		}
		case LEFT: {
			target.x -= 4;
			float length = pacmanPos.x - Maze::getInstance().getCenterOfTile(target).x;
			RectangleShape hline(Vector2f(length, 2));
			hline.setOrigin(0, hline.getLocalBounds().height / 2);
			hline.setPosition(Maze::getInstance().getCenterOfTile(target));

			hline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(hline);

			break;
		}
		case DOWN: {
			target.y += 4;
			float length = Maze::getInstance().getCenterOfTile(target).y - pacmanPos.y;
			RectangleShape vline(Vector2f(2, length));
			vline.setOrigin(vline.getLocalBounds().width / 2, 0);
			vline.setPosition(pacmanPos);

			vline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(vline);

			break;
		}
		case RIGHT: {
			target.x += 4;
			float length = Maze::getInstance().getCenterOfTile(target).x - pacmanPos.x;
			RectangleShape hline(Vector2f(length, 2));
			hline.setOrigin(0, hline.getLocalBounds().height / 2);
			hline.setPosition(pacmanPos);

			hline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(hline);

			break;
		}
		}
		for (RectangleShape line : lines)
			window->draw(line);
	}

	Ghost::drawTargetingSystem(window);
}

Vector2i Pinky::getScatterTargetPosition() const {
	return Vector2i(2,0);
}

Vector2i Pinky::getChaseTargetPosition() const {
	Vector2i target = PacMan::getInstance().getGridPos();
	switch (PacMan::getInstance().getCurrentDirection()) {
	case UP:
		target.x -= 4;
		target.y -= 4;
		break;
	case LEFT:
		target.x -= 4;
		break;
	case DOWN:
		target.y += 4;
		break;
	case RIGHT:
		target.x += 4;
		break;
	}
	return target;
}

Vector2i Pinky::getRevivePosition() const {
	return Vector2i(14, 17);
}

void Pinky::moveToOriginalStartingPosition() {
	gridPos = Vector2i(13, 17);
	currentDirection = DOWN;
	mainSprite.setTexture(downTexture->operator[](0));
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
	mainSprite.move(HALFTILE, 0);
	isSpawned = isOutside = false;
}