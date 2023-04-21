#include "GameTextures.h"

GameTextures GameTextures::instance;

GameTextures::GameTextures() {
	Context context;
	loadMazeTexture();

	loadGhostDirectionalTextures();
	loadGhostEyesTextures();
	loadGhostFrightenedTextures();

	loadPacManTextures();

	loadFruitsAndPointsTextures();

	loadGhostBonusPointsTextures();
	
	loadOtherTextures();

	loadGhostIconTextures();
	loadPacManIconTexture();
	loadPacManFigureIconTexture();

	loadPacManEatingTexture();
	loadStuckBlinkyPartsTextures();
	loadWoundedBlinkyTextures();
	loadPatchedUpBlinkyTextures();
	loadBlinkyTrueFormTextures();
}

GameTextures& GameTextures::getInstance() {
	return instance;
}

//-----------------Load Textures---------------------
void GameTextures::loadMazeTexture() {
	mazeTexture.loadFromFile(pmSpritesFile, IntRect(1, 0, 224, 248));
}

void GameTextures::loadGhostDirectionalTextures() {
	for (size_t j = 0; j < 4; j++) {
		for (size_t i = 0; i < 8; i++) {
			Texture ghostTexture;
			if (ghostTexture.loadFromFile(pmSpritesFile, IntRect(X_TO_LOAD_SPRITE + i * 16, 65 + 16 * j, 14, 14)))
				ghostDirectionalTextures[j][i >> 1].push_back(ghostTexture);
		}
	}
}

void GameTextures::loadGhostFrightenedTextures() {
	Texture blue1, blue2, white1, white2;
	blue1.loadFromFile(pmSpritesFile, IntRect(358, 65, 14, 14));
	blue2.loadFromFile(pmSpritesFile, IntRect(374, 65, 14, 14));
	white1.loadFromFile(pmSpritesFile, IntRect(390, 65, 14, 14));
	white2.loadFromFile(pmSpritesFile, IntRect(406, 65, 14, 14));

	blueFrightenedTextures = { blue1, blue2 };
	flickeringFrightenedTextures = { blue1, blue2, white1, white2 };

}

void GameTextures::loadGhostEyesTextures() {
	for (size_t i = 0; i < 4; i++) {
		Texture eyes;
		if (eyes.loadFromFile(pmSpritesFile, IntRect(358 + i * 16, PINKY_SPRITE_Y, 14, 14)))
			ghostEyeTextures.push_back(eyes);
	}
}

void GameTextures::loadGhostBonusPointsTextures() {
	const int numHeight = 11;

	ghostBonusScore_Textures.resize(4);
	ghostBonusScore_Textures[0].loadFromFile(pmSpritesFile, IntRect(X_TO_LOAD_SPRITE, CLYDE_SPRITE_Y + 18, 14, numHeight));

	ghostBonusScore_Textures[1].loadFromFile(pmSpritesFile, IntRect(X_TO_LOAD_SPRITE + 15, CLYDE_SPRITE_Y + 18, 15, numHeight));
	ghostBonusScore_Textures[2].loadFromFile(pmSpritesFile, IntRect(X_TO_LOAD_SPRITE + 31, CLYDE_SPRITE_Y + 18, 15, numHeight));

	ghostBonusScore_Textures[3].loadFromFile(pmSpritesFile, IntRect(X_TO_LOAD_SPRITE + 46, CLYDE_SPRITE_Y + 18, 18, numHeight));
}

void GameTextures::loadPacManTextures() {
	auto loadTexture = [](int yToLoadTexture, vector <Texture>& textures) {
		for (size_t i = 0; i < 3; i++) {
			Texture pacman;
			pacman.loadFromFile(pmSpritesFile, IntRect(X_TO_LOAD_SPRITE + (i << 4), yToLoadTexture, 13, 13));
			textures.push_back(pacman);

		}
	};
	loadTexture(1, pacmanTextures[3]);
	loadTexture(17, pacmanTextures[1]);
	loadTexture(33, pacmanTextures[0]);
	loadTexture(49, pacmanTextures[2]);

	for (size_t i = 0; i < 11; i++) {
		Texture died;
		died.loadFromFile(pmSpritesFile, IntRect(278 + i * 16, 3, 13, 13));
			pacmanTextures[4].push_back(died);
	}
}

void GameTextures::loadFruitsAndPointsTextures() {
	for (int i = 0; i < 10; i++) 
		bonusItemsTextures[i].resize(((i != 1) ? 1 : 2));

	bonusItemsTextures[0][0].loadFromFile(pmSpritesFile, IntRect(228, 243, 2, 2));
	
	bonusItemsTextures[1][0].loadFromFile(pmSpritesFile, IntRect(233, 240, 8, 8));
	bonusItemsTextures[1][1].loadFromFile(pmSpritesFile, sf::IntRect(230, 160, 8, 8));
	
	for (int i = CHERRY; i <= KEY; i++) {
		bonusItemsTextures[i][0].loadFromFile(pmSpritesFile, IntRect(278 + (i - CHERRY) * 16, 49, 13, 13));
	}
	
	fruitPointsTextures.resize(8);
	for (int i = CHERRY; i <= KEY; i++) {
		IntRect rect;
		const float top = 146, height = 11;
		rect.height = height;
		rect.top = top;
		switch (i) {
		case CHERRY: {	//100
			rect.left = X_TO_LOAD_SPRITE + 1;
			rect.width = 14;
			break;
		}
		case STRAWBERRY: { //300
			rect.left = X_TO_LOAD_SPRITE + 15;
			rect.width = 15;
			break;
		}
		case ORANGE: { //500
			rect.left = X_TO_LOAD_SPRITE + 31;
			rect.width = 15;
			break;
		}
		case APPLE: { //700
			rect.left = X_TO_LOAD_SPRITE + 46;
			rect.width = 16;
			break;
		}
		case MELON: { //1000
			rect.left = X_TO_LOAD_SPRITE + 63;
			rect.width = 20;
			break;
		}
		case GALAXIAN_FLAGSHIP: { //2000
			rect.left = X_TO_LOAD_SPRITE + 61;
			rect.width = 20;
			rect.top = top + 16;
			break;
		}
		case BELL: { //3000
			rect.left = X_TO_LOAD_SPRITE + 61;
			rect.width = 20;
			rect.top = top + 32;
			break;
		}
		case KEY: { //5000
			rect.left = X_TO_LOAD_SPRITE + 61;
			rect.width = 20;
			rect.top = top + 48;
			break;
		}
		}

		fruitPointsTextures[i - CHERRY].loadFromFile(pmSpritesFile, rect);

	}

}

void GameTextures::loadOtherTextures() {
	gameoverTexture.loadFromFile(ready_gameoverFile, IntRect(0, 0, 175, 22));

	readyTexture.loadFromFile(ready_gameoverFile, IntRect(180, 5, 98, 20));

	titleTexture.loadFromFile("Resources/Textures/pacman_title.png");

}

void GameTextures::loadGhostIconTextures() {
	ghostIconTextures.resize(4);
	string color[4] = { "red","pink", "blue", "orange" };

	for (int i = 0; i < 4; i++) 
		ghostIconTextures[i].loadFromFile("Resources/Textures/3d-pixel-ghost-" + color[i] + ".png");
}

void GameTextures::loadPacManIconTexture() {
	pacmanIconTexture.loadFromFile("Resources/Textures/pac_man_icon.png");
}

void GameTextures::loadPacManFigureIconTexture() {
	pacmanFigureIconTexture.loadFromFile("Resources/Textures/pacman.png");
}

void GameTextures::loadPacManEatingTexture() {
	pacmanEatingTexture.loadFromFile("Resources/Textures/pacman_eating.png");
}

void GameTextures::loadStuckBlinkyPartsTextures(){
	stuckBlinkyPartsTextures.resize(5);
	const float commonY = 98, commonHeight = 13;
	stuckBlinkyPartsTextures[0].loadFromFile(pmSpritesFile, IntRect(371, commonY, 2, commonHeight));
	stuckBlinkyPartsTextures[1].loadFromFile(pmSpritesFile, IntRect(382, commonY, 9, commonHeight));
	stuckBlinkyPartsTextures[2].loadFromFile(pmSpritesFile, IntRect(394, commonY, 12, commonHeight));
	stuckBlinkyPartsTextures[3].loadFromFile(pmSpritesFile, IntRect(407, commonY, 15, commonHeight));
	stuckBlinkyPartsTextures[4].loadFromFile(pmSpritesFile, IntRect(429, commonY, 10, commonHeight));
	
}

void GameTextures::loadWoundedBlinkyTextures() {
	woundedBlinkyTextures.resize(2);
	woundedBlinkyTextures[0].loadFromFile(pmSpritesFile, IntRect(358, 113, 14, 14));
	woundedBlinkyTextures[1].loadFromFile(pmSpritesFile, IntRect(374, 113, 14, 14));
}

void GameTextures::loadPatchedUpBlinkyTextures() {
	patchedUpBlinkyTextures.resize(2);
	patchedUpBlinkyTextures[0].loadFromFile(pmSpritesFile, IntRect(390, 113, 14, 14));
	patchedUpBlinkyTextures[1].loadFromFile(pmSpritesFile, IntRect(406, 113, 14, 14));
}

void GameTextures::loadBlinkyTrueFormTextures() {
	blinkyTrueFormTextures.resize(2);
	blinkyTrueFormTextures[0].loadFromFile(pmSpritesFile, IntRect(366, CLYDE_SPRITE_Y + 16, 22, 14));
	blinkyTrueFormTextures[1].loadFromFile(pmSpritesFile, IntRect(397, CLYDE_SPRITE_Y + 16, 22, 14));
}

//-------------------Get Textures---------------
const Texture* GameTextures::getMazeTexture() const {
	return &mazeTexture;
}

const vector<Texture>* GameTextures::getGhostEyesTextures() const {
	return &ghostEyeTextures;
}

const vector<Texture>* GameTextures::getBlueFrightenedTextures() const {
	return &blueFrightenedTextures;
}

const vector <Texture>* GameTextures::getFlickeringFrightenedTextures() const {
	return &flickeringFrightenedTextures;
}

//===========Blinky==============
const vector<Texture>* GameTextures::getUpBlinky() const {
	return &ghostDirectionalTextures[0][2];
}

const vector<Texture>* GameTextures::getLeftBlinky() const {
	return &ghostDirectionalTextures[0][1];
}

const vector<Texture>* GameTextures::getDownBlinky() const {
	return &ghostDirectionalTextures[0][3];
}

const vector<Texture>* GameTextures::getRightBlinky() const {
	return &ghostDirectionalTextures[0][0];
}

//============Pinky==============
const vector<Texture>* GameTextures::getUpPinky() const {
	return &ghostDirectionalTextures[1][2];
}

const vector<Texture>* GameTextures::getLeftPinky() const {
	return &ghostDirectionalTextures[1][1];
}

const vector<Texture>* GameTextures::getDownPinky() const {
	return &ghostDirectionalTextures[1][3];
}

const vector<Texture>* GameTextures::getRightPinky() const {
	return &ghostDirectionalTextures[1][0];
}

//============Inky==============
const vector<Texture>* GameTextures::getUpInky() const {
	return &ghostDirectionalTextures[2][2];
}

const vector<Texture>* GameTextures::getLeftInky() const {
	return &ghostDirectionalTextures[2][1];
}

const vector<Texture>* GameTextures::getDownInky() const {
	return &ghostDirectionalTextures[2][3];
}

const vector<Texture>* GameTextures::getRightInky() const {
	return &ghostDirectionalTextures[2][0];
}

//==========Clyde==============
const vector<Texture>* GameTextures::getUpClyde() const {
	return &ghostDirectionalTextures[3][2];
}

const vector<Texture>* GameTextures::getLeftClyde() const {
	return &ghostDirectionalTextures[3][1];
}

const vector<Texture>* GameTextures::getDownClyde() const {
	return &ghostDirectionalTextures[3][3];
}

const vector<Texture>* GameTextures::getRightClyde() const {
	return &ghostDirectionalTextures[3][0];
}

//==========PacMan============
const vector<Texture>* GameTextures::getUpPacMan() const {
	return &pacmanTextures[0];
}

const vector<Texture>* GameTextures::getLeftPacMan() const {
	return &pacmanTextures[1];
}

const vector<Texture>* GameTextures::getDownPacMan() const {
	return &pacmanTextures[2];
}

const vector<Texture>* GameTextures::getRightPacMan() const {
	return &pacmanTextures[3];
}

const vector<Texture>* GameTextures::getDiedPacMan() const {
	return &pacmanTextures[4];
}

const Texture* GameTextures::getPacManLiveTexture() const {
	return &pacmanTextures[1][1];
}

//===========Bonus Items============
const vector<Texture>* GameTextures::getBonusItemsTexture(BonusItems item) const {
	return &bonusItemsTextures[item];
}

const Texture* GameTextures::getFruitPointsTexture(BonusItems fruitItem) const {
	if (fruitItem < CHERRY || fruitItem > KEY)
		return NULL;
	return &fruitPointsTextures[fruitItem - CHERRY];
}

const vector<Texture>* GameTextures::getGhostBonusPointsTextures() const {
	return &ghostBonusScore_Textures;
}

//=============Other textures==========
const Texture* GameTextures::getGameReadyTexture() const {
	return &readyTexture;
}

const Texture* GameTextures::getGameOverTexture() const {
	return &gameoverTexture;
}

const Texture* GameTextures::getPacManTitleTexture() const {
	return &titleTexture;
}

const Texture* GameTextures::getGhostIconTextures(size_t ghostType) const {
	return &ghostIconTextures[ghostType];
}

const Texture* GameTextures::getPacManIconTexture() const {
	return &pacmanIconTexture;
}

const Texture* GameTextures::getPacManFigureIconTexture() const {
	return &pacmanFigureIconTexture;
}

const Texture* GameTextures::getPacManEatingTexture() const {
	return &pacmanEatingTexture;
}

const vector <Texture>* GameTextures::getStuckBlinkyPartsTextures() const{
	return &stuckBlinkyPartsTextures;
}

const vector<Texture>* GameTextures::getWoundedBlinkyTextures() const {
	return &woundedBlinkyTextures;
}

const vector<Texture>* GameTextures::getPatchedUpBlinkyTextures() const {
	return &patchedUpBlinkyTextures;
}

const vector<Texture>* GameTextures::getBlinkyTrueFormTextures() const{
	return &blinkyTrueFormTextures;
}