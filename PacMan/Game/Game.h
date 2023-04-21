#pragma once
#include "../SoundEffect/SoundEffect.h"
#include "../Entities/Ghost/Blinky.h"
#include "../Entities/Ghost/Pinky.h"
#include "../Entities/Ghost/Inky.h"
#include "../Entities/Ghost/Clyde.h"
#include "../Entities/PacMan/PacMan.h"
#include "../Entities/BonusItem/BonusItem.h"

constexpr uint8_t MAXGHOST = 4;

enum TimerForGameEvent{
	PacManEatingGhost, GameReadyState, ShowFruitBonusPoints, PacManStopsEatingPellet, 
	LevelComplete, FruitAppearance
};

class Game{
private:
	//==============Time for timer================
	float eatingTimer, readyStateTimer, showFruitPointsTimer;
	float PacManNotEatingPelletTimer, fruitAppearTimer;
	float lvlCompleteTimer;

	//==============Game's sprites===============
	Sprite readySprite, gameoverSprite, fruitBonusPointsSprite;
	vector <Sprite> ghostBonusScore_Sprites;
	short currentIndexOfScores;
	vector <Sprite> fruitsAtBRCorner;

	//============Boolean variables==============
	bool IS_PAUSING, IS_GAMEOVER;
	bool isInReadyState;
	bool restartLevelWhenLost;
	bool isShowingFruitBonusPoints;

	bool regenerateGhost;

	bool isPacManInvincible;

	bool InfiniteLives;

	//===========Variables managing the order of getting-outside ghosts==============
	int8_t globalDotCounter;
	bool activateGlobalDotCounter;
	uint8_t currentPreferredGhost;
	vector <pair <bool, float>> revivedGhostOrderToActivateDotCounter;
	const vector <float> maxTimeWaitingPMNotEating = { 0,0,4,4 };

	//===========Containers of entities==========
	vector <Ghost*> ghostGroup;
	map <pair <int, int>, BonusItem*> itemList;

	//==========Managing Bonus Items============
	size_t previousNumOfPellets;
	unordered_set <pair <int, int>, HashFunctionForPairII> powerPelletLocations, pelletLocations, fruitLocations;
	
	//===========Sound management===============
	SoundEffect* soundManager;
	short currentSpurtMoveSound;

	//===========Level management==============
	Level* level;

	//============Scores and Lives============
	size_t scores, previousScores;
	short* numOfLives;

	//===============Utility methods=================
	void loadSprites();
	void loadFruitsAtBRCornerSprites();
	void setupNewGame();
	void updateTimer(TimerForGameEvent type, float deltaTime);
	void soundManagement();
	void decideWhichGhostWhenToLeaveHome(float deltaTime);

	void update(float deltaTime);

	void checkPellets();

public:
	Game(bool InfiniteLives = true, short *maxLives = NULL);
	~Game();
	void startGame();
	void processMainGame(float deltaTime);
	void restartLostGame();
	void restartLevel();
	void pauseGame(bool pause);
	void pauseSoundEffect(bool pause = true, short type = -1);
	void levelUp();
	void skipLevel();
	
	void draw(RenderWindow* window) const;
	
	//==========Setters===========
	void setPacManInvincible(bool isInvincible);
	void setShowPath(bool showPath);
	void setShowTarget(bool showTarget);

	//==========Getters===========
	size_t getCurrentScores() const;
	uint32_t getCurrentLevel() const;
	bool isGameOver() const;
};