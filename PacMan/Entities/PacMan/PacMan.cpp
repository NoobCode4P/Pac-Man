#include "PacMan.h"

PacMan PacMan::instance;

PacMan::PacMan(){
	this->gridPos = Vector2i(13, 26);
	state = true;
	isEatingGhost = isEatingPellet = isEatingPowerPellet = isEatingFruit = false;

	inputDirection = NONE;
	Context context;
	loadDirectionalTexture();
	currentDirection = LEFT;
	newGridPos = Vector2i(gridPos.x - 1, gridPos.y);
	isMoving = false;
	pelletLocations = NULL;
	isInvincible = false;
	
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos).x + HALFTILE, Maze::getInstance().getCenterOfTile(gridPos).y);
}

void PacMan::moveToOriginalStartingPosition() {
	gridPos = Vector2i(13, 26);
	isEatingGhost = isEatingPellet = isEatingPowerPellet = isEatingFruit = false;
	isGridPosUpdated = false;
	inputDirection = NONE;
	currentDirection = LEFT;

	mainSprite.setTexture(GameTextures::getInstance().getRightPacMan()->operator[](2));
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos).x + HALFTILE, Maze::getInstance().getCenterOfTile(gridPos).y);
	newGridPos = Vector2i(gridPos.x + 1, gridPos.y);
	isMoving = false;
}

PacMan& PacMan::getInstance() {
	return instance;
}

void PacMan::loadDirectionalTexture() {
	mainSprite.setTexture(GameTextures::getInstance().getRightPacMan()->operator[](2));
	mainSprite.setOrigin(mainSprite.getLocalBounds().width / 2, mainSprite.getLocalBounds().height / 2);
	mainSprite.setScale(COMMON_SCALE, COMMON_SCALE);
}

void PacMan::setMainTexture() {
	mainSprite.setTexture(GameTextures::getInstance().getRightPacMan()->operator[](2));
}

const Texture* PacMan::getTexture() const {
	return &GameTextures::getInstance().getRightPacMan()->operator[](1);
}

Directions PacMan::getCurrentDirection() const {
	return currentDirection;
}

Vector2f PacMan::getPosition() const { 
	return mainSprite.getPosition(); 
}

bool PacMan::isAlive() const {
	return state;
}

bool PacMan::isEnergized() const {
	return isPoweredUp;
}

void PacMan::eatGhost() {
	isEatingGhost = true;
}

void PacMan::finishEatingGhost() {
	isEatingGhost = false;
}

void PacMan::die() {
	state = false;
	animation.setSwitchTime(0.14f);
	animation.setShortClip(GameTextures::getInstance().getDiedPacMan());
}

void PacMan::revive() {
	state = true;
}

void PacMan::setInvincible(bool isInvincible) {
	this->isInvincible = isInvincible;
}

void PacMan::restartAtLocation(Vector2i pos) {
	isEatingGhost = isEatingPellet = isEatingPowerPellet = isEatingFruit = false;
	gridPos = pos;
	newGridPos = Vector2i(gridPos.x - 1, gridPos.y);
	isGridPosUpdated = false;
	isMoving = false;
	pelletLocations = powerPelletLocations = fruitLocations = NULL;
	currentDirection = LEFT;
	inputDirection = NONE;
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(pos));
}

void PacMan::becomeEnergized() {
	isPoweredUp = true;
}

void PacMan::draw(RenderWindow* window) {
	if (visible)
		window->draw(mainSprite);
}

void PacMan::setPelletsLocation(unordered_set <pair <int, int>, HashFunctionForPairII>& locations) {
	pelletLocations = &locations;
}

void PacMan::setBonusItemLocations(vector <unordered_set <pair <int, int>, HashFunctionForPairII>*> bonusItemLocations) {
	pelletLocations = bonusItemLocations[0];
	powerPelletLocations = bonusItemLocations[1];
	fruitLocations = bonusItemLocations[2];
}

Directions PacMan::getUserInput() {
	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
		return UP;
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		return LEFT;
	if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
		return DOWN;
	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		return RIGHT;
	return NONE;
}

void PacMan::update(float deltaTime) {
	if (state) {
		if (!isEatingGhost) {
			inputDirection = getUserInput();

			Move(deltaTime);
		}
		updateAnimation();
	}
	if (isMoving || !state) {
		animation.update(deltaTime);
	}
}

void PacMan::Move(float deltaTime) {
	float delta = deltaTime * COMMON_SPEED * (curLvl ? curLvl->getGhostSpeedPercentage() : 1);

	auto checkIfEatsPellet = [this]()->bool {
		return pelletLocations->find(make_pair(gridPos.x, gridPos.y)) != pelletLocations->end();
	};

	auto checkIfEatsPowerPellet = [this]()->bool {
		return powerPelletLocations->find(make_pair(gridPos.x, gridPos.y)) != powerPelletLocations->end();
	};

	auto checkIfEatsFruit = [this]()->bool {
		return fruitLocations->find(make_pair(gridPos.x, gridPos.y)) != fruitLocations->end();
	};

	auto isEatingBonusItem = [checkIfEatsPellet, checkIfEatsPowerPellet, checkIfEatsFruit, this]()-> pair <int,int> {
		if (!pelletLocations || !powerPelletLocations || !fruitLocations) {
			isEatingPellet = isEatingPowerPellet = isEatingFruit = false;
			return make_pair(-1, -1);
		}

		if (checkIfEatsPellet()) {
			isEatingPellet = true;
			isEatingPowerPellet = isEatingFruit = false;
			pelletLocations->erase(make_pair(gridPos.x, gridPos.y));
			return make_pair(gridPos.x, gridPos.y);
		}	
		else if (checkIfEatsPowerPellet()) {
			isEatingPowerPellet = true;
			isEatingPellet = isEatingFruit = false;
			powerPelletLocations->erase(make_pair(gridPos.x, gridPos.y));
			return make_pair(gridPos.x, gridPos.y);
		}
		else if (checkIfEatsFruit()) {
			isEatingFruit = true;
			isEatingPellet = isEatingPowerPellet = false;
			fruitLocations->erase(make_pair(gridPos.x, gridPos.y));
			return make_pair(gridPos.x, gridPos.y);
		}
		isEatingPellet = isEatingPowerPellet = isEatingFruit = false;
		return make_pair(-1, -1);
	};

	switch (currentDirection) {
	case NONE:
		pickNextTile();
		break;
	case UP: {
		Vector2f centerOfUpTile = Maze::getInstance().getCenterOfTile(newGridPos);
		if (mainSprite.getPosition().y - delta >= centerOfUpTile.y) {
			mainSprite.move(0, -delta);
			if (mainSprite.getPosition().y < centerOfUpTile.y + HALFTILE && !isGridPosUpdated) {
				gridPos = newGridPos;
				isGridPosUpdated = true;

				eatenItemPos = isEatingBonusItem();
			}
		}
		else
			pickNextTile();
		break;
	}
	case LEFT: {
		Vector2f centerOfLeftTile = Maze::getInstance().getCenterOfTile((newGridPos.x == NUMBER_OF_COLUMNS - 1 && !isGridPosUpdated) ? Vector2i(-1,newGridPos.y): newGridPos);
		if (mainSprite.getPosition().x - delta >= centerOfLeftTile.x) {
			mainSprite.move(-delta, 0);

			if (mainSprite.getPosition().x < centerOfLeftTile.x + HALFTILE && !isGridPosUpdated) {
				Vector2i tmp = gridPos;
				gridPos = newGridPos;
				isGridPosUpdated = true;

				if (Maze::getInstance().isPairOfPortals(make_pair(gridPos, tmp)))
					teleportHorizontallyTo(gridPos, false);

				eatenItemPos = isEatingBonusItem();
			}
		}
		else
			pickNextTile();
		break;
	}
	case DOWN: {
		Vector2f centerOfDownTile = Maze::getInstance().getCenterOfTile(newGridPos);
		if (mainSprite.getPosition().y + delta <= centerOfDownTile.y) {
			mainSprite.move(0, delta);
			if (mainSprite.getPosition().y > centerOfDownTile.y - HALFTILE && !isGridPosUpdated) {
				gridPos = newGridPos;
				isGridPosUpdated = true;

				eatenItemPos = isEatingBonusItem();
			}
		}
		else
			pickNextTile();
		break;
	}
	case RIGHT: {
		Vector2f centerOfRightTile = Maze::getInstance().getCenterOfTile((newGridPos.x == 0 && !isGridPosUpdated) ? Vector2i(NUMBER_OF_COLUMNS, newGridPos.y) : newGridPos);
		if (mainSprite.getPosition().x + delta <= centerOfRightTile.x) {
			mainSprite.move(delta, 0);
			if (mainSprite.getPosition().x > centerOfRightTile.x - HALFTILE && !isGridPosUpdated) {
				Vector2i tmp = gridPos;
				gridPos = newGridPos;
				isGridPosUpdated = true;

				if (Maze::getInstance().isPairOfPortals(make_pair(gridPos, tmp)))
					teleportHorizontallyTo(gridPos, true);

				eatenItemPos = isEatingBonusItem();
			}
		}
		else
			pickNextTile();
		break;
	}
	}
}

void PacMan::pickNextTile() {
	bool isPicked = false;
	if (inputDirection != NONE) {
		switch (inputDirection) {
		case UP: {
			Vector2i candidatePos(gridPos.x, gridPos.y - 1);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				currentDirection = UP;
				newGridPos = candidatePos;
				isPicked = true;
			}
			break;
		}
		case LEFT: {
			Vector2i candidatePos(gridPos.x == 0 ? NUMBER_OF_COLUMNS - 1 : gridPos.x - 1, gridPos.y);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				currentDirection = LEFT;
				newGridPos = candidatePos;
				isPicked = true;
			}
			break;
		}
		case DOWN: {
			Vector2i candidatePos(gridPos.x, gridPos.y + 1);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				currentDirection = DOWN;
				newGridPos = candidatePos;
				isPicked = true;
			}
			break;
		}
		case RIGHT: {
			Vector2i candidatePos(gridPos.x == NUMBER_OF_COLUMNS - 1 ? 0 : gridPos.x + 1, gridPos.y);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				currentDirection = RIGHT;
				newGridPos = candidatePos;
				isPicked = true;
			}
			break;
		}
		}
	}
	if (inputDirection == NONE || !isPicked) {
		switch (currentDirection) {
		case UP: {
			Vector2i candidatePos(gridPos.x, gridPos.y - 1);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				newGridPos = candidatePos;
			}
			break;
		}
		case LEFT: {
			Vector2i candidatePos(gridPos.x == 0 ? NUMBER_OF_COLUMNS - 1 : gridPos.x - 1, gridPos.y);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				newGridPos = candidatePos;
			}
			break;
		}
		case DOWN: {
			Vector2i candidatePos(gridPos.x, gridPos.y + 1);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				newGridPos = candidatePos;
			}
			break;
		}
		case RIGHT: {
			Vector2i candidatePos(gridPos.x == NUMBER_OF_COLUMNS - 1 ? 0 : (gridPos.x + 1), gridPos.y);
			if (Maze::getInstance().isWalkableForPacMan(candidatePos)) {
				newGridPos = candidatePos;
			}
			break;
		}
		}
	}
	
	isGridPosUpdated = false;
	isMoving = (gridPos != newGridPos);
}

void PacMan::updateAnimation() {
	if (state) {
		animation.setSwitchTime(0.07f);
		switch (currentDirection) {
		case UP:
			animation.setShortClip(GameTextures::getInstance().getUpPacMan());
			break;
		case LEFT:
			animation.setShortClip(GameTextures::getInstance().getLeftPacMan());
			break;
		case DOWN:
			animation.setShortClip(GameTextures::getInstance().getDownPacMan());
			break;
		case RIGHT:
			animation.setShortClip(GameTextures::getInstance().getRightPacMan());
			break;
		}
	}
}