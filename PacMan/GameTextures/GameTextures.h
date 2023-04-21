#pragma once
#include "../Common.h"

const string pmSpritesFile = "Resources/Textures/PacManSprites.png";
const string ready_gameoverFile = "Resources/Textures/ready-gameover.png";


class GameTextures {
	Texture mazeTexture;

	vector <Texture> ghostEyeTextures;
	vector <Texture> blueFrightenedTextures, flickeringFrightenedTextures;
	vector <Texture> ghostDirectionalTextures [4][4]; // row: Blinky Pinky Inky Clyde, col: right left up down

	vector <Texture> pacmanTextures[5]; // up left down right died

	vector <Texture> ghostBonusScore_Textures;
	
	vector <Texture> bonusItemsTextures[10], fruitPointsTextures;

	Texture readyTexture, gameoverTexture, titleTexture;
	Texture pacmanIconTexture;
	Texture pacmanFigureIconTexture;
	Texture pacmanEatingTexture;
	vector <Texture> ghostIconTextures;

	vector <Texture> stuckBlinkyPartsTextures, woundedBlinkyTextures, patchedUpBlinkyTextures, blinkyTrueFormTextures;

	//==========LOAD TEXTURES FUNCTIONS==============

	void loadMazeTexture();

	void loadGhostDirectionalTextures();
	void loadGhostFrightenedTextures();
	void loadGhostEyesTextures();

	void loadPacManTextures();

	void loadFruitsAndPointsTextures();

	void loadGhostBonusPointsTextures();

	void loadOtherTextures();

	void loadGhostIconTextures();
	void loadPacManIconTexture();
	void loadPacManFigureIconTexture();
	void loadPacManEatingTexture();

	void loadStuckBlinkyPartsTextures();
	void loadWoundedBlinkyTextures();
	void loadPatchedUpBlinkyTextures();
	void loadBlinkyTrueFormTextures();

	GameTextures();

	static GameTextures instance;
public:
	static GameTextures& getInstance();

	const Texture* getMazeTexture() const;

	const vector <Texture>* getGhostEyesTextures() const;
	const vector <Texture>* getBlueFrightenedTextures() const;
	const vector <Texture>* getFlickeringFrightenedTextures() const;
	
	const vector <Texture>* getUpBlinky() const;
	const vector <Texture>* getLeftBlinky() const;
	const vector <Texture>* getDownBlinky() const;
	const vector <Texture>* getRightBlinky() const;

	const vector <Texture>* getUpPinky() const;
	const vector <Texture>* getLeftPinky() const;
	const vector <Texture>* getDownPinky() const;
	const vector <Texture>* getRightPinky() const;

	const vector <Texture>* getUpInky() const;
	const vector <Texture>* getLeftInky() const;
	const vector <Texture>* getDownInky() const;
	const vector <Texture>* getRightInky() const;

	const vector <Texture>* getUpClyde() const;
	const vector <Texture>* getLeftClyde() const;
	const vector <Texture>* getDownClyde() const;
	const vector <Texture>* getRightClyde() const;

	const vector <Texture>* getUpPacMan() const;
	const vector <Texture>* getLeftPacMan() const;
	const vector <Texture>* getDownPacMan() const;
	const vector <Texture>* getRightPacMan() const;
	const vector <Texture>* getDiedPacMan() const;

	const Texture* getPacManLiveTexture() const;

	const vector <Texture>* getBonusItemsTexture(BonusItems item) const;
	const Texture* getFruitPointsTexture(BonusItems fruitItem) const;

	const vector <Texture>* getGhostBonusPointsTextures() const;
	const Texture* getGameReadyTexture()const;
	const Texture* getGameOverTexture()const;
	const Texture* getPacManTitleTexture() const;

	const Texture* getGhostIconTextures(size_t ghostType) const;
	const Texture* getPacManIconTexture() const;
	const Texture* getPacManFigureIconTexture() const;
	const Texture* getPacManEatingTexture() const;
	const vector <Texture>* getStuckBlinkyPartsTextures() const;
	const vector <Texture>* getWoundedBlinkyTextures() const;
	const vector <Texture>* getPatchedUpBlinkyTextures() const;
	const vector <Texture>* getBlinkyTrueFormTextures() const;
};