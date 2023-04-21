#include "Inky.h"

Inky::Inky(Vector2i gridPos, Ghost* ghost, bool applyStatesInterchange) : Ghost(gridPos, applyStatesInterchange) {
	blinky = ghost;
	loadDirectionalTexture();
	
	if (!Maze::getInstance().isInHouse(gridPos)) {
		mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
		isSpawned = isOutside = true;
	}
	else 
		moveToOriginalStartingPosition();

	pathSketcher->setColor(Color(TURQUOISE_COLOR));
	targetTile.setFillColor(Color::Cyan);
}

Inky::Inky(Ghost* ghost) : Ghost(Vector2i(11,17)){
	blinky = ghost;
	loadDirectionalTexture();
	moveToOriginalStartingPosition();
	pathSketcher->setColor(Color(TURQUOISE_COLOR));
	targetTile.setFillColor(Color::Cyan);
}

Inky::~Inky(){}

void Inky::loadDirectionalTexture() {
	upTexture = GameTextures::getInstance().getUpInky();
	leftTexture = GameTextures::getInstance().getLeftInky();
	downTexture = GameTextures::getInstance().getDownInky();
	rightTexture = GameTextures::getInstance().getRightInky();

	mainSprite.setTexture(upTexture->operator[](0));
	mainSprite.setOrigin(mainSprite.getLocalBounds().width / 2, mainSprite.getLocalBounds().height / 2);
	mainSprite.setScale(COMMON_SCALE, COMMON_SCALE);
}

void Inky::drawTargetingSystem(RenderWindow* window) {
	if (getState() == CHASE && isSpawned) {
		Vector2i midPos = PacMan::getInstance().getGridPos();
		Vector2i blinkyPos = blinky ? blinky->getGridPos() : Vector2i(100, 100);
		Vector2f pacmanPos = PacMan::getInstance().getPosition();

		vector <RectangleShape> lines;
		CircleShape point(5);
		point.setOrigin(point.getLocalBounds().width / 2, point.getLocalBounds().height / 2);
		point.setFillColor(Color(SILVER_COLOR));

		switch (PacMan::getInstance().getCurrentDirection()) {
		case UP: {
			midPos.y -= 2;
			float length = pacmanPos.y - Maze::getInstance().getCenterOfTile(midPos).y;
			float length2 = Maze::getInstance().getCenterOfTile(midPos).x - Maze::getInstance().getCenterOfTile(Vector2i(midPos.x - 2, midPos.y)).x;
			midPos.x -= 2;
			RectangleShape vline(Vector2f(2, length)), hline(Vector2f(length2, 2));
			vline.setOrigin(vline.getLocalBounds().width / 2, vline.getLocalBounds().height);
			vline.setPosition(pacmanPos);

			hline.setOrigin(0, hline.getLocalBounds().height / 2);
			hline.setPosition(Maze::getInstance().getCenterOfTile(midPos));

			vline.setFillColor(Color(SILVER_COLOR));
			hline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(vline);
			lines.push_back(hline);

			break;
		}
		case LEFT: {
			midPos.x -= 2;
			float length = pacmanPos.x - Maze::getInstance().getCenterOfTile(midPos).x;
			RectangleShape hline(Vector2f(length, 2));
			hline.setOrigin(0, hline.getLocalBounds().height / 2);
			hline.setPosition(Maze::getInstance().getCenterOfTile(midPos));

			hline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(hline);

			break;
		}
		case DOWN: {
			midPos.y += 2;
			float length = Maze::getInstance().getCenterOfTile(midPos).y - pacmanPos.y;
			RectangleShape vline(Vector2f(2, length));
			vline.setOrigin(vline.getLocalBounds().width / 2, 0);
			vline.setPosition(pacmanPos);

			vline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(vline);

			break;
		}
		case RIGHT: {
			midPos.x += 2;
			float length = Maze::getInstance().getCenterOfTile(midPos).x - pacmanPos.x;
			RectangleShape hline(Vector2f(length, 2));
			hline.setOrigin(0, hline.getLocalBounds().height / 2);
			hline.setPosition(pacmanPos);

			hline.setFillColor(Color(SILVER_COLOR));

			lines.push_back(hline);

			break;
		}
		}

		point.setPosition(Maze::getInstance().getCenterOfTile(midPos));

		Vector2i inkyPos(2 * midPos.x - blinkyPos.x, 2 * midPos.y - blinkyPos.y);

		Vertex line2[] = {
			Vertex(Maze::getInstance().getCenterOfTile(blinkyPos)),
			Vertex(Maze::getInstance().getCenterOfTile(inkyPos))
		};
		line2[0].color = Color(SILVER_COLOR);
		line2[1].color = Color(SILVER_COLOR);
		for (RectangleShape line : lines)
			window->draw(line);
		window->draw(line2, 2, Lines);
		window->draw(point);
	}

	Ghost::drawTargetingSystem(window);
}

Vector2i Inky::getScatterTargetPosition() const {
	return Vector2i(NUMBER_OF_COLUMNS - 1, NUMBER_OF_ROWS - 1);
}

Vector2i Inky::getChaseTargetPosition() const {
	Vector2i midPos = PacMan::getInstance().getGridPos();
	Vector2i blinkyPos = blinky ? blinky->getGridPos() : Vector2i(100,100);
	
	switch (PacMan::getInstance().getCurrentDirection()) {
	case UP:
		midPos.x -= 2;
		midPos.y -= 2;
		break;
	case LEFT:
		midPos.x -= 2;
		break;
	case DOWN:
		midPos.y += 2;
		break;
	case RIGHT:
		midPos.x += 2;
		break;
	}
	
	return Vector2i(2 * midPos.x - blinkyPos.x, 2 * midPos.y - blinkyPos.y);
}

Vector2i Inky::getRevivePosition() const {
	return Vector2i(11, 17);
}

void Inky::moveOutOfTheHouse(float delta) {
	float middleVerticalLine = Maze::getInstance().getRightCoordOfTile(Vector2i(13, 17)) + 1;
	if (mainSprite.getPosition().x + delta < middleVerticalLine) {
		currentDirection = RIGHT;
		mainSprite.move(delta, 0);
	}
	else
		Ghost::moveOutOfTheHouse(delta);
}

void Inky::moveIntoTheHouse(float delta) {
	float lowerLimit = Maze::getInstance().getCenterOfTile(Vector2i(13, 17)).y;
	if (mainSprite.getPosition().y + delta < lowerLimit) {
		currentDirection = DOWN;
		mainSprite.move(0, delta);
	}
	else {
		float limitLine = Maze::getInstance().getRightCoordOfTile(Vector2i(11, 17));
		if (mainSprite.getPosition().x - delta > limitLine) {
			currentDirection = LEFT;
			mainSprite.move(-delta, 0);
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

void Inky::moveToOriginalStartingPosition(){
	gridPos = Vector2i(11, 17);
	currentDirection = UP;
	mainSprite.setTexture(upTexture->operator[](0));
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
	mainSprite.move(HALFTILE, 0);
	isSpawned = isOutside = false;
}