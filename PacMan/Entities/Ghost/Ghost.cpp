#include "Ghost.h"

Ghost::Ghost(Vector2i gridPos, bool applyStatesInterchange) {
	this->gridPos = gridPos;
	this->applyStatesInterchange = applyStatesInterchange;
	currentIndexInPath = 0;

	pathSketcher = new GraphicsPath();
	pathSketcher->setPath(currentPath);

	targetTile.setSize((Vector2f(8, 8)));
	targetTile.setOrigin(targetTile.getLocalBounds().width / 2, targetTile.getLocalBounds().height / 2);

	state = SCATTER;
	isReversedDirection = false;

	frightenedTime = 0;
	waveTime = 0;
	currentWave = 0;

	isHomeAccessDenied = true;
	activateDotCounter = false;
	isEatenGhostInFrontOfTheGate = false;
	eatenByPacMan = false;
	
	currentTarget = gridPos;
	showPath = showTarget = false;
}

Ghost::~Ghost() {
	delete pathSketcher;
}

void Ghost::ShowPath(bool showPath) {
	this->showPath = showPath;
}

void Ghost::ShowTarget(bool showTarget) {
	this->showTarget = showTarget;
}

void Ghost::becomeFrightened() {
	state = FRIGHTENED;
	reverseDirection();
	animation.setShortClip(GameTextures::getInstance().getBlueFrightenedTextures());
	frightenedTime = 0;
}

void Ghost::reverseDirection() {
	isReversedDirection = true;
	currentDirection = getOppositeDirection(currentDirection);
	isGridPosUpdated = !isGridPosUpdated;
}

void Ghost::resetData() {
	state = SCATTER;
	isReversedDirection = false;
	isEatenGhostInFrontOfTheGate = false;
	eatenByPacMan = false;
	isHomeAccessDenied = true;
	currentIndexInPath = 0;
	currentPath.clear();
	currentTarget = gridPos;
	pathSketcher->clearPath();

	frightenedTime = 0;
	waveTime = 0;
	currentWave = 0;
}

GhostStates Ghost::getState() const {
	return state; 
}

void Ghost::setState(GhostStates ghostState){
	state = ghostState;
}

const vector <Texture>& Ghost::getTextures(Directions direction) const {
	switch (direction) {
	case UP:
		return *upTexture;
	case LEFT:
		return *leftTexture;
	case DOWN:
		return *downTexture;
	case RIGHT:
		return *rightTexture;
	}
	return *downTexture;
}

bool Ghost::isInsideHouse() const {
	return !isOutside;
}

void Ghost::spawn() {
	this->isSpawned = true;
}

void Ghost::setWaveDurations(const vector <float>& wave) {
	waveDurations = wave;
}

void Ghost::setDotCounter(int8_t dotCounterLimt) {
	dotCounter = dotCounterLimt;
}

void Ghost::countDots() {
	dotCounter--;
}

bool Ghost::reachesDotLimit() const {
	return dotCounter <= 0;
}

void Ghost::setEatenByPacMan(bool eatenByPacMan){
	this->eatenByPacMan = eatenByPacMan;
}

void Ghost::drawGhost(RenderWindow* window) {
	if (visible) {
		if (showPath && state != FRIGHTENED)
			pathSketcher->drawPath(window);

		if (showTarget)
			drawTargetingSystem(window);

		window->draw(mainSprite);
	}
}

Vector2f Ghost::getGhostPosition() const{
	return mainSprite.getPosition();
}

bool Ghost::isEatenByPacMan() const {
	return eatenByPacMan;
}

void Ghost::drawTargetingSystem(RenderWindow* window) {
	if (isSpawned) {
		switch (state) {
		case CHASE:
			targetTile.setPosition(Maze::getInstance().getCenterOfTile(getChaseTargetPosition()));
			break;
		case SCATTER:
			targetTile.setPosition(Maze::getInstance().getCenterOfTile(getScatterTargetPosition()));
			break;
		case EATEN: case FRIGHTENED:
			targetTile.setPosition(Maze::getInstance().getCenterOfTile(currentTarget));
			break;
		}

		window->draw(targetTile);
	}
}

void Ghost::moveAlongMazePath(float delta) {
	switch (currentDirection) {
	case NONE:
		pickNextTile();
		break;
	case UP: {
		Vector2f centerOfUpTile = Maze::getInstance().getCenterOfTile(Vector2i(gridPos.x, gridPos.y - !isGridPosUpdated));
		if (mainSprite.getPosition().y - delta >= centerOfUpTile.y) {
			mainSprite.move(0, -delta);
			if (mainSprite.getPosition().y < centerOfUpTile.y + HALFTILE && !isGridPosUpdated) {
				if (!isReversedDirection)
					gridPos = currentPath[++currentIndexInPath];
				else {
					gridPos.y -= 1;
					isReversedDirection = false;
					currentPath.clear();
				}
				isGridPosUpdated = true;
			}
		}
		else
			pickNextTile();
		
		break;
	}
	case LEFT: {
		Vector2f centerOfLeftTile = Maze::getInstance().getCenterOfTile(Vector2i(gridPos.x - !isGridPosUpdated, gridPos.y));
		
		if (mainSprite.getPosition().x - delta >= centerOfLeftTile.x) {
			mainSprite.move(-delta, 0);
			if (mainSprite.getPosition().x < centerOfLeftTile.x + HALFTILE && !isGridPosUpdated) {
				if (!isReversedDirection) {
					gridPos = currentPath[++currentIndexInPath];

					if (Maze::getInstance().isPairOfPortals(make_pair(currentPath[currentIndexInPath - 1], gridPos)))
						teleportHorizontallyTo(gridPos, false);
				}
				else {
					if (Maze::getInstance().isPortal(gridPos))
						teleportHorizontallyTo(gridPos = Vector2i(27, 17), false);
					else
						gridPos.x -= 1;
					isReversedDirection = false;
					currentPath.clear();
				}
				isGridPosUpdated = true;
			}
		}
		else
			pickNextTile();
		break;
	}
	case DOWN: {
		Vector2f centerOfDownTile = Maze::getInstance().getCenterOfTile(Vector2i(gridPos.x, gridPos.y + !isGridPosUpdated));
		if (mainSprite.getPosition().y + delta <= centerOfDownTile.y) {
			mainSprite.move(0, delta);
			if (mainSprite.getPosition().y > centerOfDownTile.y - HALFTILE && !isGridPosUpdated) {
				if (!isReversedDirection)
					gridPos = currentPath[++currentIndexInPath];
				else {
					gridPos.y += 1;
					isReversedDirection = false;
					currentPath.clear();
				}
				isGridPosUpdated = true;
			}
		}
		else
			pickNextTile();
		break;
	}
	case RIGHT: {
		Vector2f centerOfRightTile = Maze::getInstance().getCenterOfTile(Vector2i(gridPos.x + !isGridPosUpdated, gridPos.y));
		if (mainSprite.getPosition().x + delta <= centerOfRightTile.x) {
			mainSprite.move(delta, 0);
			if (mainSprite.getPosition().x > centerOfRightTile.x - HALFTILE && !isGridPosUpdated) {
				if (!isReversedDirection) {
					gridPos = currentPath[++currentIndexInPath];

					if (Maze::getInstance().isPairOfPortals(make_pair(gridPos, currentPath[currentIndexInPath - 1])))
						teleportHorizontallyTo(gridPos, true);
				}
				else {
					if (Maze::getInstance().isPortal(gridPos))
						teleportHorizontallyTo(gridPos = Vector2i(0, 17), true);
					else
						gridPos.x += 1;
					isReversedDirection = false;
					currentPath.clear();
				}
				isGridPosUpdated = true;
			}
		}
		else
			pickNextTile();
		break;
	}
	}
}

void Ghost::pickNextTile() {
	Vector2i newTarget;
	
	switch (state) {
	case SCATTER:
		newTarget = getScatterTargetPosition();
		break;
	case CHASE:
		newTarget = getChaseTargetPosition();
		break;
	case FRIGHTENED:
		newTarget = Maze::getInstance().getMazePath()->getRandomAccessibleAdjacentPosition(gridPos, currentDirection);
		break;
	case EATEN:
		newTarget = (currentDirection == LEFT) ? Vector2i(14, 14) : Vector2i(13, 14);
		break;
	}

	auto findNewPath = [this](const Vector2i& newTarget) {
		currentTarget = newTarget;
		currentPath.clear();
		if (Maze::getInstance().isInMaze(newTarget)) {
			currentPath = Maze::getInstance().getMazePath()->findPath(gridPos, currentTarget, currentDirection, (state == SCATTER || state == CHASE), isHomeAccessDenied);
			
			if (currentPath.empty())
				currentPath = Maze::getInstance().getMazePath()->findPathToNearAccessibleTile(gridPos, currentTarget, currentDirection, (state == SCATTER || state == CHASE), isHomeAccessDenied);
			
		}
		else
			currentPath = Maze::getInstance().getMazePath()->findPathToNearAccessibleTile(gridPos, currentTarget, currentDirection, (state == SCATTER || state == CHASE), isHomeAccessDenied);
		
		currentIndexInPath = 0;

		pathSketcher->renderPath();
	};

	if (currentTarget != newTarget || currentPath.empty()) {	// If the target is new then find a new path
		findNewPath(newTarget);
	}

	if (currentIndexInPath == currentPath.size() - 1) {
		currentPath.clear();
		switch (state) {
		case EATEN: {
			isEatenGhostInFrontOfTheGate = true;
			
			break;
		}
		case FRIGHTENED: {
			currentPath = vector <Vector2i>{ gridPos, Maze::getInstance().getMazePath()->getRandomAccessibleAdjacentPosition(gridPos, currentDirection) };
			currentTarget = currentPath[1];
			break;
		}
		default: {
			currentPath = Maze::getInstance().getMazePath()->findPathToNearAccessibleTile(gridPos, currentTarget, currentDirection, (state == SCATTER || state == CHASE), isHomeAccessDenied);
			break;
		}
		}
		
		if (!currentPath.empty() && state != FRIGHTENED)
			pathSketcher->renderPath();
		else
			pathSketcher->clearPath();

		currentIndexInPath = 0;
	}

	else if (currentIndexInPath < currentPath.size() - 1) {
		Vector2i newPos = currentPath[currentIndexInPath + 1];
		
		if (newPos.y < gridPos.y)
			currentDirection = UP;
		else if (newPos.x < gridPos.x)
			currentDirection = LEFT;
		else if (newPos.y > gridPos.y)
			currentDirection = DOWN;
		else if (newPos.x > gridPos.x)
			currentDirection = RIGHT;

		if (Maze::getInstance().isPairOfPortals(make_pair(newPos, gridPos))) {
			if (newPos.x < gridPos.x)
				currentDirection = RIGHT;
			else if (newPos.x > gridPos.x)
				currentDirection = LEFT;
		}
		isGridPosUpdated = false;
	}
	
}

void Ghost::updateAnimation() {
	switch (state) {
	case FRIGHTENED: {
		if (frightenedTime >= curLvl->getMaxGhostFrightenedDuration() * 0.6)
			animation.setShortClip(GameTextures::getInstance().getFlickeringFrightenedTextures());
		else
			animation.setShortClip(GameTextures::getInstance().getBlueFrightenedTextures());
		break;
	}
	case EATEN: {
		switch (currentDirection) {
		case UP:
			mainSprite.setTexture(GameTextures::getInstance().getGhostEyesTextures()->operator[](2));
			break;
		case LEFT:
			mainSprite.setTexture(GameTextures::getInstance().getGhostEyesTextures()->operator[](1));
			break;
		case DOWN:
			mainSprite.setTexture(GameTextures::getInstance().getGhostEyesTextures()->operator[](3));
			break;
		case RIGHT:
			mainSprite.setTexture(GameTextures::getInstance().getGhostEyesTextures()->operator[](0));
			break;	
		}
		break;
	}
	default: {
		switch (currentDirection) {
		case UP:
			animation.setShortClip(upTexture);
			break;
		case LEFT:
			animation.setShortClip(leftTexture);
			break;
		case DOWN:
			animation.setShortClip(downTexture);
			break;
		case RIGHT:
			animation.setShortClip(rightTexture);
			break;
		}
		break;
	}
	}	
}

void Ghost::update(float deltaTime) {
	if (freezed)
		return;

	if (applyStatesInterchange) {
		switch (state) {
		case EATEN: {
			if (gridPos == getRevivePosition()) {
				state = static_cast <GhostStates> (currentWave & 1);
			}
			break;
		}
		case FRIGHTENED: {
			frightenedTime += deltaTime;

			if (frightenedTime >= curLvl->getMaxGhostFrightenedDuration()) {
				frightenedTime = 0;

				state = static_cast <GhostStates> (currentWave & 1);
			}
			break;
		}
		default: {
			if (waveDurations[currentWave] != -1) {
				waveTime += deltaTime;
				if (waveTime >= waveDurations[currentWave]) {
					waveTime = 0;

					currentWave++;

					state = static_cast <GhostStates> (currentWave & 1);
					reverseDirection();
				}
			}

			break;
		}
		}
	}
	
	float delta = deltaTime * COMMON_SPEED * (curLvl ? curLvl->getGhostSpeedPercentage() : 1);

	switch (state) {
	case FRIGHTENED:
		delta *= (curLvl) ? curLvl->getFrightenedGhostSpeedPercentage() : 0.5f;
		break;
	case EATEN:
		delta *= 3.5;
		break;
	}
	if (Maze::getInstance().isInTunnel(gridPos) && state != EATEN)
		delta *= (curLvl) ? curLvl->getGhostTunnelSpeedPercentage() : 0.4f;

	if (!isOutside) {
		if (isSpawned)
			moveOutOfTheHouse(delta);
		else 
			bounceInHouse(delta);
		
	}
	else {
		if (state == EATEN && isEatenGhostInFrontOfTheGate) {
			if (isHomeAccessDenied)
				procedureToGetInsideHouse(delta);
			else
				moveIntoTheHouse(delta);

		}
		else {
			moveAlongMazePath(delta);

			if (showPath)
				pathSketcher->setCurrentIndex(currentIndexInPath);
		}

	}
	updateAnimation();

	if (state != EATEN)
		animation.update(deltaTime);

	auto isCollidedWith = [this](const PacMan& pacman) {return gridPos == pacman.getGridPos(); };

	if (isCollidedWith(PacMan::getInstance())) {
		switch (state) {
		case FRIGHTENED: {
			state = EATEN;
			PacMan::getInstance().eatGhost();
			visible = false;
			freezed = true;
			eatenByPacMan = true;
			PacMan::getInstance().setVisible(false);

			if (gridPos != Vector2i(13, 14) && gridPos != Vector2i(14, 14))
				isEatenGhostInFrontOfTheGate = false;
			else
				isEatenGhostInFrontOfTheGate = true;

			break;
		}
		case EATEN: {

			break;
		}
		default: {
			if (!PacMan::getInstance().isInvincible)
				PacMan::getInstance().die();
			break;
		}
		}
	}
}

void Ghost::procedureToGetInsideHouse(float delta) {
	float middleVerticalLine = Maze::getInstance().getLeftCoordOfTile(Vector2i(14, 14));

	if (gridPos == Vector2i(13, 14)) {
		if (mainSprite.getPosition().x + delta < middleVerticalLine) {
			mainSprite.move(delta, 0);
		}
		else {
			isHomeAccessDenied = false;
		}
	}
	else if (gridPos == Vector2i(14, 14)) {
		if (mainSprite.getPosition().x - delta > middleVerticalLine) {
			mainSprite.move(-delta, 0);
		}
		else {
			isHomeAccessDenied = false;
		}
	}
}

void Ghost::moveOutOfTheHouse(float delta) {
	float upperLimit = Maze::getInstance().getCenterOfTile(Vector2i(13, 14)).y;

	if (mainSprite.getPosition().y - delta > upperLimit) {
		mainSprite.move(0, -delta);
		currentDirection = UP;
	}
	else {
		isOutside = true;
		gridPos = Vector2i(13, 14);
		isHomeAccessDenied = true;
		currentDirection = NONE;
	}
}

void Ghost::moveIntoTheHouse(float delta) {
	float lowerLimit = Maze::getInstance().getCenterOfTile(Vector2i(13, 17)).y;
	if (mainSprite.getPosition().y + delta < lowerLimit) {
		mainSprite.move(0, delta);
		currentDirection = DOWN;
	}
	else {
		isOutside = false;
		isSpawned = false;
		gridPos = getRevivePosition();
		currentDirection = UP;
		activateDotCounter = true;
	}

}

void Ghost::bounceInHouse(float delta) {
	float middleYCoords = Maze::getInstance().getCenterOfTile(gridPos).y;

	auto isSpawnedConditionMet = [this]()->bool {
		return (activateDotCounter && dotCounter <= 0) || isSpawned;
	};

	auto resetWhenSpawnedConditionIsMet = [this]() {
		activateDotCounter = false;
		isSpawned = true;
	};

	switch (currentDirection) {
	case UP: {
		float bottomOfUpTile = Maze::getInstance().getBottomCoordOfTile(Vector2i(gridPos.x, gridPos.y - 1));
		if (mainSprite.getPosition().y < bottomOfUpTile) {
			currentDirection = DOWN;
		}
		else {
			mainSprite.move(0, -delta);
			if (mainSprite.getPosition().y <= middleYCoords && isSpawnedConditionMet()) {
				resetWhenSpawnedConditionIsMet();
			}
		}
		break;
	}
	case DOWN: {
		float topOfDownTile = Maze::getInstance().getTopCoordOfTile(Vector2i(gridPos.x, gridPos.y + 1));
		if (mainSprite.getPosition().y > topOfDownTile) {
			currentDirection = UP;
		}
		else {
			mainSprite.move(0, delta);
			if (mainSprite.getPosition().y >= middleYCoords && isSpawnedConditionMet()) {
				resetWhenSpawnedConditionIsMet();
			}
		}
		break;
	}
	}
}