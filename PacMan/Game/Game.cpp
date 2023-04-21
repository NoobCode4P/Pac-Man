#include "Game.h"

Game::Game(bool InfiniteLives, short *maxLives) {
	IS_PAUSING = false;
	IS_GAMEOVER = false;
	this->InfiniteLives = InfiniteLives;

	numOfLives = maxLives;
	eatingTimer = readyStateTimer = showFruitPointsTimer = PacManNotEatingPelletTimer = fruitAppearTimer = 0;
	
	scores = previousScores = 0;

	ghostGroup.push_back(new Blinky());
	ghostGroup.push_back(new Pinky());
	ghostGroup.push_back(new Inky(ghostGroup[0]));
	ghostGroup.push_back(new Clyde());

	level = new Level();
	soundManager = &SoundEffect::getManager();

	loadSprites();
}

Game::~Game() {
	for (Ghost* ghost : ghostGroup)
		delete ghost;

	for (map <pair <int, int>, BonusItem*>::iterator it = itemList.begin(); it != itemList.end(); it++)
		delete it->second;

	soundManager->stopPlayingSound();
	delete level;
}

void Game::draw(RenderWindow* window) const {
	Maze::getInstance().drawMaze(window);

	if (IS_GAMEOVER)
		window->draw(gameoverSprite);

	for (map <pair <int, int>, BonusItem*>::const_iterator it = itemList.begin(); it != itemList.end(); it++)
		it->second->draw(window);
	
	for (int i = level->getDrawFruitOnScreenIdxFromTo().first; i <= level->getDrawFruitOnScreenIdxFromTo().second; i++)
		window->draw(fruitsAtBRCorner[i]);

	if (IS_GAMEOVER)	// To save time for not checking any events other than the above ones when game is over
		return;

	if (isShowingFruitBonusPoints)
		window->draw(fruitBonusPointsSprite);

	PacMan::getInstance().draw(window);

	for (Ghost* ghost : ghostGroup)
		ghost->drawGhost(window);

	if (PacMan::getInstance().isEatingGhost)
		window->draw(ghostBonusScore_Sprites[currentIndexOfScores == 0 ? 3 : currentIndexOfScores - 1]);


	if (isInReadyState)
		window->draw(readySprite);

}

void Game::loadSprites() {
	//============GameOverText==============
	gameoverSprite.setTexture(*GameTextures::getInstance().getGameOverTexture());
	gameoverSprite.setOrigin(gameoverSprite.getLocalBounds().width / 2, gameoverSprite.getLocalBounds().height / 2);
	gameoverSprite.setPosition(Maze::getInstance().getLeftCoordOfTile(Vector2i(14,20)), Maze::getInstance().getCenterOfTile(Vector2i(14,20)).y);
	
	//============ReadyText==============
	readySprite.setTexture(*GameTextures::getInstance().getGameReadyTexture());
	readySprite.setScale(1.1f,1.1f);
	readySprite.setOrigin(readySprite.getLocalBounds().width / 2, readySprite.getLocalBounds().height / 2);
	readySprite.setPosition(Maze::getInstance().getLeftCoordOfTile(Vector2i(14, 20)), Maze::getInstance().getCenterOfTile(Vector2i(14, 20)).y);

	//============Ghost scores: 200 400 800 1600==============
	for (int i = 0; i < GameTextures::getInstance().getGhostBonusPointsTextures()->size(); i++) {
		Sprite s(GameTextures::getInstance().getGhostBonusPointsTextures()->operator[](i));
		s.setOrigin(s.getLocalBounds().width / 2, s.getLocalBounds().height / 2);
		s.setScale(2, 2);
		ghostBonusScore_Sprites.push_back(s);
	}

	//============Fruits at the bottom right of the maze==========
	fruitsAtBRCorner.resize(7);

	float originX = Maze::getInstance().getLeftCoordOfTile(Vector2i(27, 34));
	float originY = Maze::getInstance().getTopCoordOfTile(Vector2i(1, 35));
	for (int i = 0; i < 7; i++) {
		fruitsAtBRCorner[i].setTexture(GameTextures::getInstance().getBonusItemsTexture(APPLE)->operator[](0));
		fruitsAtBRCorner[i].setOrigin(fruitsAtBRCorner[i].getGlobalBounds().width / 2, fruitsAtBRCorner[i].getGlobalBounds().height / 2);
		fruitsAtBRCorner[i].setScale(COMMON_SCALE, COMMON_SCALE);
		fruitsAtBRCorner[i].setPosition(originX - i * COMMON_SCALE * TILESIZE * 2, originY);
	}
	
}

void Game::loadFruitsAtBRCornerSprites() {
	vector <BonusItems> arr = level->getFruitsAtBottomRight();
	
	for (int i = 0; i < arr.size(); i++) 
		fruitsAtBRCorner[i].setTexture(GameTextures::getInstance().getBonusItemsTexture(arr[i])->operator[](0));
}

bool Game::isGameOver()const {
	return	IS_GAMEOVER;
}

void Game::checkPellets() {
	if (pelletLocations.size() == 170 || pelletLocations.size() == 70) {
		fruitAppearTimer = 0;
		itemList[make_pair(13, 20)] = new BonusItem(Vector2i(13, 20), level->getBonusSymbol());
		fruitLocations.insert(make_pair(13, 20));
	}

	if (PacMan::getInstance().isEatingPowerPellet) {
		PacMan::getInstance().becomeEnergized();
		if ((1 <= level->getCurrentLevel() && level->getCurrentLevel() <= 16) || level->getCurrentLevel() == 18) {
			for (Ghost* ghost : ghostGroup) {
				if (ghost->getState() != EATEN)
					ghost->becomeFrightened();
			}
		}
		else {
			for (Ghost* ghost : ghostGroup) {
				ghost->reverseDirection();
			}
		}

		delete itemList[PacMan::getInstance().eatenItemPos];
		itemList.erase(PacMan::getInstance().eatenItemPos);

		PacMan::getInstance().isEatingPowerPellet = false;

		currentIndexOfScores = 0;

		scores += 50;
	}

	else if (PacMan::getInstance().isEatingPellet) {
		pair <int, int> eatenItemPos = PacMan::getInstance().eatenItemPos;
		if (itemList.find(eatenItemPos) != itemList.end()) {
			delete itemList[eatenItemPos];
			itemList.erase(eatenItemPos);
			scores += 10;
		}
	}
	else if (PacMan::getInstance().isEatingFruit) {
		pair <int, int> eatenItemPos = PacMan::getInstance().eatenItemPos;

		fruitBonusPointsSprite = itemList[eatenItemPos]->getPointsSprite();
		
		isShowingFruitBonusPoints = true;

		scores += static_cast <size_t> (itemList[eatenItemPos]->getBonusPoints());

		delete itemList[eatenItemPos];
		itemList.erase(eatenItemPos);

		fruitAppearTimer = 0;
	}
	
	if (PacMan::getInstance().isMoving) {
		if (PacMan::getInstance().isEatingPellet) {
			if (!soundManager->isPlaying(SOUND_EATPELLET))
				soundManager->playSound(SOUND_EATPELLET, false);
		}
		else {
			if (soundManager->isPlaying(SOUND_EATPELLET))
				soundManager->stopPlayingSound(SOUND_EATPELLET);
		}
		if (PacMan::getInstance().isEatingFruit) {
			if (!soundManager->isPlaying(SOUND_EATFRUIT))
				soundManager->playSound(SOUND_EATFRUIT, false);
			PacMan::getInstance().isEatingFruit = false;
		}
		
	}
	else {
		if (soundManager->isPlaying(SOUND_EATPELLET))
			soundManager->stopPlayingSound(SOUND_EATPELLET);
		if (soundManager->isPlaying(SOUND_EATFRUIT))
			soundManager->stopPlayingSound(SOUND_EATFRUIT);
	}

}

void Game::pauseGame(bool pause){
	IS_PAUSING = pause;
}

void Game::pauseSoundEffect(bool pause, short type) {
	if (pause)
		soundManager->pauseSound(type);
	else
		soundManager->unpauseSound(type);
}

void Game::setPacManInvincible(bool isInvincible) {
	isPacManInvincible = isInvincible;
	PacMan::getInstance().setInvincible(isInvincible);
}

void Game::setShowPath(bool showPath) {
	for (Ghost* ghost : ghostGroup)
		ghost->ShowPath(showPath);
}

void Game::setShowTarget(bool showTarget) {
	for (Ghost* ghost : ghostGroup)
		ghost->ShowTarget(showTarget);
}

size_t Game::getCurrentScores() const {
	return scores;
}

uint32_t Game::getCurrentLevel() const {
	return level->getCurrentLevel();
}

void Game::update(float deltaTime) {
	if (PacMan::getInstance().isAlive()) {
		if (PacMan::getInstance().isEatingGhost) {
			updateTimer(PacManEatingGhost, deltaTime);

			for (Ghost* ghost : ghostGroup)
				if (ghost->getState() == EATEN && !ghost->isFreezed()) {
					ghost->update(deltaTime);

				}
		}
		else {
			previousNumOfPellets = pelletLocations.size();

			PacMan::getInstance().update(deltaTime);

			auto isThereAnyGhostInHouse = [this]() -> bool {
				for (const Ghost* ghost : ghostGroup)
					if (ghost->isInsideHouse())
						return true;
				return false;
			};
			if (isThereAnyGhostInHouse()) {
				decideWhichGhostWhenToLeaveHome(deltaTime);
			}
			
			for (Ghost* ghost : ghostGroup)
				ghost->update(deltaTime);
			

			if (PacMan::getInstance().isEatingGhost) {
				if (!soundManager->isPlaying(SOUND_EATGHOST))
					soundManager->playSound(SOUND_EATGHOST, false);
				for (Ghost* ghost : ghostGroup) {
					if (ghost->isEatenByPacMan()) {
						ghostBonusScore_Sprites[currentIndexOfScores++].setPosition(ghost->getGhostPosition());
						ghost->setEatenByPacMan(false);
						scores += static_cast <size_t> (currentIndexOfScores) * 200;
					}
				}
				
				if (currentIndexOfScores == 4)
					currentIndexOfScores = 0;
			}
		}
		
		soundManagement();

		if (!PacMan::getInstance().isAlive()) {
			PacMan::getInstance().isMoving = false;
			for (Ghost* ghost : ghostGroup) {
				ghost->setFreezed(true);
				ghost->setVisible(false);
			}

			soundManager->stopPlayingSound();

			if (!soundManager->isPlaying(SOUND_PACMAN_DEATH))
				soundManager->playSound(SOUND_PACMAN_DEATH, false);
		}
	}
	else {
		PacMan::getInstance().update(deltaTime);
	}
	
	for (map <pair <int, int>, BonusItem*>::iterator it = itemList.begin(); it != itemList.end(); it++)
		it->second->update(deltaTime);
	
}

void Game::soundManagement() {
	auto isStillInFrightened = [this]() -> bool {
		for (Ghost* ghost : this->ghostGroup)
			if (ghost->getState() == FRIGHTENED)
				return true;
		return false;
	};

	auto isThereEatenRetreating = [this]()->bool {
		for (Ghost* ghost : this->ghostGroup)
			if (ghost->getState() == EATEN && !ghost->isFreezed())
				return true;
		return false;
	};

	bool checkFrightenedMode = isStillInFrightened();
	bool checkEatenRetreatingMode = isThereEatenRetreating();

	auto manageSpurtMoveSound = [this]() {
		if (pelletLocations.size() <= 20) {
			currentSpurtMoveSound = SOUND_SPURT_MOVE_5;
			if (soundManager->isPlaying(SOUND_SPURT_MOVE_4))
				soundManager->stopPlayingSound(SOUND_SPURT_MOVE_4);
		}
		else if (pelletLocations.size() <= 50) {
			currentSpurtMoveSound = SOUND_SPURT_MOVE_4;
			if (soundManager->isPlaying(SOUND_SPURT_MOVE_3))
				soundManager->stopPlayingSound(SOUND_SPURT_MOVE_3);
		}
		else if (pelletLocations.size() <= 100) {
			currentSpurtMoveSound = SOUND_SPURT_MOVE_3;
			if (soundManager->isPlaying(SOUND_SPURT_MOVE_2))
				soundManager->stopPlayingSound(SOUND_SPURT_MOVE_2);
		}
		else if (pelletLocations.size() <= 140) {
			currentSpurtMoveSound = SOUND_SPURT_MOVE_2;
			if (soundManager->isPlaying(SOUND_SPURT_MOVE_1))
				soundManager->stopPlayingSound(SOUND_SPURT_MOVE_1);
		}
		else
			currentSpurtMoveSound = SOUND_SPURT_MOVE_1;
	};

	manageSpurtMoveSound();

	if (!checkFrightenedMode && !checkEatenRetreatingMode) {
		if (soundManager->isPlaying(SOUND_RETREAT))
			soundManager->stopPlayingSound(SOUND_RETREAT);

		if (soundManager->isPlaying(SOUND_POWERUP))
			soundManager->stopPlayingSound(SOUND_POWERUP);

		if (!soundManager->isPlaying(currentSpurtMoveSound))
			soundManager->playSound(currentSpurtMoveSound, true);
	}
	else {
		if (soundManager->isPlaying(currentSpurtMoveSound))
			soundManager->stopPlayingSound(currentSpurtMoveSound);

		if (!checkEatenRetreatingMode) {
			if (soundManager->isPlaying(SOUND_RETREAT))
				soundManager->stopPlayingSound(SOUND_RETREAT);

			if (checkFrightenedMode) {
				if (!soundManager->isPlaying(SOUND_POWERUP))
					soundManager->playSound(SOUND_POWERUP, true);
			}
			else {
				if (soundManager->isPlaying(SOUND_POWERUP)) {
					soundManager->stopPlayingSound(SOUND_POWERUP);
				}
			}
		}
		else {
			if (!soundManager->isPlaying(SOUND_RETREAT))
				soundManager->playSound(SOUND_RETREAT, true);
		}
	}
}

void Game::decideWhichGhostWhenToLeaveHome(float deltaTime){
	/*
	Ghost's personal dot counter is activated when a level begins and only when the ghost is in house
	Not restartLostLvl: 
		starting game: 
			if Pacman eats, start counting personal's dot counters (ComingOut order: Pinky, Inky, Clyde)
				//until the ghosts come out of the house (activateDotCounter = false)
			if Pacman stops eating, activate separate timer (ComingOut order: Inky, Clyde) // Pinky leaves as soon as the level starts
				// activateDotCounter is still true
				if time is over, spawn the ghost with preferrence order
			->As soon as the ghost is spawned, deactivate dot Counter (activateDotCounter = false) no matter what cases happen
				
		regenerate ghost:
			if Pacman eats, reactivate personal's dot counter (ComingOut order: which ever ghost's dotCounter has reached its limit first)
			
			if Pacman stops eating, activate separate timer (ComingOut order: Pinky, Inky, Clyde (if in house))
	RestartLostLvl:
		starting game: disable all ghosts's individual dot counters and activate globalDotCounter(start counting at 0)
			if Pacman eats, start counting by using a globalDotCounter and each ghost comes out based on the counter:
				at 7, Pinky comes out
				at 17, Inky comes out
				at 32, Clyde comes out
					reset and deactivate globalDotCounter
			if Pacman stops eating, activate separate timer (ComingOut order: Pinky, Inky, Clyde)

		regenerateGhost: enable ghosts's personal dot counter(activateDotCounter = true)
			if Pacman eats, reactivate personal's dot counter (ComingOut order: which ever ghost's dotCounter has reached its limit first)
			
			if Pacman stops eating, activate separate timer (ComingOut order: Pinky, Inky, Clyde (if in house))

			->As soon as the ghost is spawned, deactivate dot Counter (activateDotCounter = false)
	*/
	
	if (previousNumOfPellets - 1 == pelletLocations.size()) {
		PacManNotEatingPelletTimer = 0;

		if (regenerateGhost) {
			bool isThereARevivedGhost = false;
			bool isCurrentPrefGhostSpawned = false;

			for (int i = 0; i < ghostGroup.size(); i++) {
				if (revivedGhostOrderToActivateDotCounter[i].first) {
					if (ghostGroup[i]->isInsideHouse()) {	// Eaten ghost is inside to regenerate
						ghostGroup[i]->countDots();
						revivedGhostOrderToActivateDotCounter[i].second = 0;	// Reset every time Pac-Man eats a pellet

						if (ghostGroup[i]->reachesDotLimit()) {
							ghostGroup[i]->spawn();
							ghostGroup[i]->activateDotCounter = false;

							revivedGhostOrderToActivateDotCounter[i].first = false;
						}
												
					}
					isThereARevivedGhost = true;
				}
				else {
					if (ghostGroup[i]->isInsideHouse()) {
						if (!isCurrentPrefGhostSpawned && i == static_cast <int> (currentPreferredGhost)) {
							ghostGroup[i]->countDots();

							if (ghostGroup[i]->reachesDotLimit()) {
								ghostGroup[i]->spawn();
								ghostGroup[i]->activateDotCounter = false;
								currentPreferredGhost++;
								isCurrentPrefGhostSpawned = true;
							}
						}
					}
				}
			}
			
			if (!isThereARevivedGhost) {
				regenerateGhost = false;
			}	
		}

		else {
			if (restartLevelWhenLost) {
				if (activateGlobalDotCounter) {
					globalDotCounter++;
					if (globalDotCounter >= 7 && ghostGroup[1]->isInsideHouse()) {
						ghostGroup[1]->spawn();
						currentPreferredGhost = 2;
					}
					else if (globalDotCounter >= 17 && ghostGroup[2]->isInsideHouse()) {
						ghostGroup[2]->spawn();
						currentPreferredGhost = 3;
					}
					else if (globalDotCounter >= 32 && ghostGroup[3]->isInsideHouse()) {
						ghostGroup[3]->spawn();
						currentPreferredGhost = MAXGHOST;
						globalDotCounter = 0;
						activateGlobalDotCounter = false;
					}
				}
			}
			else {
				if (currentPreferredGhost < MAXGHOST) {
					if (ghostGroup[currentPreferredGhost]->isInsideHouse() && ghostGroup[currentPreferredGhost]->activateDotCounter) {
						ghostGroup[currentPreferredGhost]->countDots();
					}
					else {

						if (currentPreferredGhost < MAXGHOST - 1) {
							currentPreferredGhost++;
							if (ghostGroup[currentPreferredGhost]->isInsideHouse()) {
								ghostGroup[currentPreferredGhost]->activateDotCounter = true;
								ghostGroup[currentPreferredGhost]->countDots();
							}
						}
						else { // Ghost House is empty
							currentPreferredGhost = MAXGHOST;
						}
					}
				}
			}
		}
	}
	else {	// Activate timer to track the amount of time elapsed since PacMan has last eaten a pellet
		updateTimer(PacManStopsEatingPellet, deltaTime);
	}

}

void Game::updateTimer(TimerForGameEvent type, float deltaTime) {
	switch (type) {
	case PacManEatingGhost: {
		eatingTimer += deltaTime;
		if (eatingTimer >= 1) {
			eatingTimer = 0;
			PacMan::getInstance().finishEatingGhost();

			for (int i = 0; i < ghostGroup.size(); i++) {
				if (ghostGroup[i]->isFreezed() && ghostGroup[i]->getState() == EATEN) {
					ghostGroup[i]->setVisible(true);
					ghostGroup[i]->setFreezed(false);

					revivedGhostOrderToActivateDotCounter[i].first = true;

					if (soundManager->isPlaying(SOUND_POWERUP))
						soundManager->stopPlayingSound(SOUND_POWERUP);

					soundManager->playSound(SOUND_RETREAT, true);
				}
			}
			PacMan::getInstance().setVisible(true);

			regenerateGhost = true;

		}
		break;
	}

	case GameReadyState: {
		readyStateTimer += deltaTime;
		if (readyStateTimer >= 5) {
			readyStateTimer = 0;
			isInReadyState = false;

			if (!soundManager->isPlaying(currentSpurtMoveSound))
				soundManager->playSound(currentSpurtMoveSound, true);
		}

		break;
	}

	case ShowFruitBonusPoints: {
		showFruitPointsTimer += deltaTime;
		if (showFruitPointsTimer >= 3) {
			isShowingFruitBonusPoints = false;
			showFruitPointsTimer = 0;
		}
		break;
	}

	case PacManStopsEatingPellet: {

		PacManNotEatingPelletTimer += deltaTime;

		if (regenerateGhost) {
			bool isCurrentPrefGhostSpawned = false;
			bool isThereARevivedGhost = false;
			for (int i = 0; i < ghostGroup.size(); i++) {
				if (revivedGhostOrderToActivateDotCounter[i].first) {
					if (ghostGroup[i]->isInsideHouse()) {
						revivedGhostOrderToActivateDotCounter[i].second += deltaTime;

						if (revivedGhostOrderToActivateDotCounter[i].second >= maxTimeWaitingPMNotEating[i]) {
							ghostGroup[i]->spawn();
							ghostGroup[i]->activateDotCounter = false;

							revivedGhostOrderToActivateDotCounter[i] = { false, 0.f };
						}

					}
					isThereARevivedGhost = true;
				}
				else {
					if (ghostGroup[i]->isInsideHouse()) {
						if (!isCurrentPrefGhostSpawned && i == static_cast <int> (currentPreferredGhost)) {
							if (PacManNotEatingPelletTimer >= 4) {

								PacManNotEatingPelletTimer = 0;

								ghostGroup[i]->spawn();
								ghostGroup[i]->activateDotCounter = false;
								isCurrentPrefGhostSpawned = true;
								currentPreferredGhost++;
							}

						}
					}
				}
			}
		
			if (!isThereARevivedGhost) {
				regenerateGhost = false;
			}
		}

		else {
			if (PacManNotEatingPelletTimer >= 4) {
				PacManNotEatingPelletTimer = 0;

				if (currentPreferredGhost < MAXGHOST) {
					if (ghostGroup[currentPreferredGhost]->isInsideHouse()) {
						ghostGroup[currentPreferredGhost]->spawn();

						if (!restartLevelWhenLost)
							ghostGroup[currentPreferredGhost]->activateDotCounter = false;

						currentPreferredGhost++;

						if (!restartLevelWhenLost)
							if (currentPreferredGhost <= MAXGHOST - 1)
								ghostGroup[currentPreferredGhost]->activateDotCounter = true;
					}
				}

				if (currentPreferredGhost >= MAXGHOST) {
					currentPreferredGhost = MAXGHOST;
					if (restartLevelWhenLost) {
						globalDotCounter = 0;
						activateGlobalDotCounter = false;
					}
				}

			}
		}
		break;
	}

	case LevelComplete: {
		lvlCompleteTimer += deltaTime;
		if (!soundManager->isPlaying(SOUND_EXTEND) && lvlCompleteTimer >= 3) {
			lvlCompleteTimer = 0;

			levelUp();
			soundManager->playSound(SOUND_GAMESTART, false);
		}
		else if (lvlCompleteTimer >= 1.5) {
			for (Ghost* ghost : ghostGroup)
				ghost->setVisible(false);
		}
		break;
	}

	case FruitAppearance: {
		fruitAppearTimer += deltaTime;
		if (fruitAppearTimer >= 10) {
			fruitAppearTimer = 0;
			
			fruitLocations.erase(make_pair(13, 20));

			delete itemList[make_pair(13, 20)];
			itemList.erase(make_pair(13, 20));
		}
		break;
	}

	}
}

void Game::startGame() {
	IS_PAUSING = false;
	isPacManInvincible = false;
	setupNewGame();
	soundManager->playSound(SOUND_GAMESTART, false);
}

void Game::restartLostGame() {
	restartLevelWhenLost = true;
	isInReadyState = true;

	for (Ghost* ghost : ghostGroup) {
		ghost->setFreezed(false);
		ghost->setVisible(true);
		ghost->resetData();
		ghost->moveToOriginalStartingPosition();
	}
	
	PacMan::getInstance().revive();
	PacMan::getInstance().moveToOriginalStartingPosition();

	for (map <pair <int, int>, BonusItem*>::iterator it = itemList.begin(); it != itemList.end(); it++)
		it->second->resetTexture();

	//Fruit vanishes
	if (!fruitLocations.empty()) {
		fruitAppearTimer = 0;
		fruitLocations.erase(make_pair(13, 20));

		delete itemList[make_pair(13, 20)];
		itemList.erase(make_pair(13, 20));
	}

	// Disables the ghosts'individual dot counters and uses global dot counter
	for (Ghost* ghost : ghostGroup)
		ghost->activateDotCounter = false;

	activateGlobalDotCounter = true;
	globalDotCounter = 0;

	currentPreferredGhost = 1;
}

void Game::restartLevel() {
	for (map <pair <int, int>, BonusItem*>::iterator it = itemList.begin(); it != itemList.end(); it++)
		delete it->second;

	soundManager->stopPlayingSound();

	scores = previousScores;

	setupNewGame();
	soundManager->playSound(SOUND_GAMESTART, false);
}

void Game::skipLevel() {
	for (map <pair <int, int>, BonusItem*>::iterator it = itemList.begin(); it != itemList.end(); it++)
		delete it->second;
	soundManager->stopPlayingSound();
	levelUp();
	soundManager->playSound(SOUND_GAMESTART, false);
}

void Game::setupNewGame() {
	restartLevelWhenLost = false;
	isInReadyState = true;
	isShowingFruitBonusPoints = false;
	regenerateGhost = false;
	currentIndexOfScores = 0;

	revivedGhostOrderToActivateDotCounter = { { false, 0.f }, {false, 0.f}, {false, 0.f}, {false, 0.f} };

	for (Vector2i pos : Maze::getInstance().pelletsLocations().first) {
		itemList[make_pair(pos.x, pos.y)] = new BonusItem(pos, PELLET);
		pelletLocations.insert(make_pair(pos.x, pos.y));
	}
	for (Vector2i pos : Maze::getInstance().pelletsLocations().second) {
		itemList[make_pair(pos.x, pos.y)] = new BonusItem(pos, POWER_PELLET);
		powerPelletLocations.insert(make_pair(pos.x, pos.y));
	}

	vector <unordered_set <pair <int, int>, HashFunctionForPairII>*> bonusItemLocation = {
		&pelletLocations, &powerPelletLocations, &fruitLocations
	};
	PacMan::getInstance().setBonusItemLocations(bonusItemLocation);

	
	for (int i = 0; i < ghostGroup.size(); i++) {
		ghostGroup[i]->setFreezed(false);
		ghostGroup[i]->setVisible(true);
		ghostGroup[i]->resetData();
		ghostGroup[i]->moveToOriginalStartingPosition();
		ghostGroup[i]->setCurrentLevelData(level);
		ghostGroup[i]->setWaveDurations(level->getWaveEffect());
		ghostGroup[i]->setDotCounter(level->getDotCounterLimit()[i]);
		ghostGroup[i]->activateDotCounter = false;
	}

	PacMan::getInstance().setCurrentLevelData(level);
	PacMan::getInstance().setInvincible(isPacManInvincible);
	PacMan::getInstance().revive();
	PacMan::getInstance().setVisible(true);
	PacMan::getInstance().moveToOriginalStartingPosition();

	currentPreferredGhost = 1;
	ghostGroup[currentPreferredGhost]->activateDotCounter = true;

	currentSpurtMoveSound = SOUND_SPURT_MOVE_1;

	loadFruitsAtBRCornerSprites();
}

void Game::processMainGame(float deltaTime) {
	if (IS_PAUSING || IS_GAMEOVER)
		return;

	if (!itemList.empty()) {
		if (isInReadyState) {
			if (!soundManager->isPlaying(SOUND_GAMESTART)) {
				if (restartLevelWhenLost) {
					updateTimer(GameReadyState, deltaTime);
				}
				else {
					isInReadyState = false;
					if (!soundManager->isPlaying(currentSpurtMoveSound))
						soundManager->playSound(currentSpurtMoveSound, true);
				}
			}
			
		}
		else {
			update(deltaTime);
			checkPellets();
		}

		if (isShowingFruitBonusPoints)
			updateTimer(ShowFruitBonusPoints, deltaTime);

		if (!fruitLocations.empty()) {
			updateTimer(FruitAppearance, deltaTime);
		}

		if (!PacMan::getInstance().isAlive() && !soundManager->isPlaying(SOUND_PACMAN_DEATH)) {
			if (!InfiniteLives) {
				(*numOfLives)--;
				if (*numOfLives == 0) {
					IS_GAMEOVER = true;
					PacMan::getInstance().setVisible(false);
					return;
				}
			}
			restartLostGame();
		}

		if (itemList.empty()) {
			for (Ghost* ghost : ghostGroup) {
				ghost->setFreezed(true);
			}
			soundManager->stopPlayingSound();
			PacMan::getInstance().setMainTexture();
			soundManager->playSound(SOUND_EXTEND, false);
		}
	}
	else 
		updateTimer(LevelComplete, deltaTime);
	
}

void Game::levelUp() {
	level->setLevel(level->getCurrentLevel() + 1);
	previousScores = scores;
	setupNewGame();
}