#include "Level.h"

Level::Level(uint32_t level) {
	currentLevel = level;

	idxFromTo.resize(19);
	for (int i = 0, j = 1; i < 19; i++) {
		if (i < 7)
			idxFromTo[i] = make_pair(0, i);
		else {
			idxFromTo[i] = make_pair(j, i);
			j++;
		}
	}

}

vector <BonusItems> Level::fruitAtBRScreen = {
	CHERRY, STRAWBERRY, ORANGE, ORANGE, APPLE, APPLE,
	MELON, MELON, GALAXIAN_FLAGSHIP, GALAXIAN_FLAGSHIP,
	BELL, BELL, 
	KEY, KEY, KEY, KEY, KEY, KEY, KEY
};

void Level::setLevel(uint32_t level) {
	currentLevel = level;
}

uint32_t Level::getCurrentLevel() const{
	return currentLevel;
}

float Level::getGhostSpeedPercentage() const {
	if (currentLevel == 1)
		return 0.75f;
	if (2 <= currentLevel && currentLevel <= 4)
		return 0.85f;
	return 0.95f;
}

float Level::getFrightenedGhostSpeedPercentage() const {
	if (currentLevel == 1)
		return 0.5f;
	if (2 <= currentLevel && currentLevel <= 4)
		return 0.55f;
	if ((5 <= currentLevel && currentLevel <= 16) || currentLevel == 18)
		return 0.6f;
	return 0.1f;
}

float Level::getGhostTunnelSpeedPercentage() const {
	if (currentLevel == 1)
		return 0.4f;
	if (2 <= currentLevel && currentLevel <= 4)
		return 0.45f;
	return 0.5f;
}

float Level::getPacManSpeedPercentage() const {
	if (currentLevel == 1)
		return 0.8f;
	if ((2 <= currentLevel && currentLevel <= 4) || currentLevel >= 21)
		return 0.9f;
	return 1.f;
}

float Level::getMaxGhostFrightenedDuration() const {
	switch (currentLevel) {
	case 1:
		return 10.f;
	case 2: case 6:
		return 9.f;
	case 3:
		return 8.f;
	case 4: 
		return 7.f;
	case 5: case 7:
		return 6.f;
	case 8: case 9: case 10:
		return 5.f;
	case 11: case 12: case 13: case 15:
		return 4.f;
	case 14: case 16: case 18:
		return 3.f;
	}
	return 0;
}

vector <float> Level::getWaveEffect() const {
	if (currentLevel == 1)
		return { 7.f,20.f,7.f,20.f,5.f,20.f,5.f,-1.f };
	if (2 <= currentLevel && currentLevel <= 4)
		return { 7.f,20.f,7.f,20.f,5.f,1033.f,0.016f,-1.f };
	return { 5.f,20.f,5.f,20.f,5.f,1037.f,0.016f,-1.f };
}

vector <int8_t> Level::getDotCounterLimit()const {
	switch (currentLevel) {
	case 1: 
		return { 0, 0, 30, 60 };
	case 2:
		return { 0, 0, 0, 50 };
	default:
		return { 0,0,0,0 };
	}
}

BonusItems Level::getBonusSymbol() const {
	switch (currentLevel) {
	case 1:
		return CHERRY;
	case 2:
		return STRAWBERRY;
	case 3: case 4:
		return ORANGE;
	case 5: case 6:
		return APPLE;
	case 7: case 8:
		return MELON;
	case 9: case 10:
		return GALAXIAN_FLAGSHIP;
	case 11: case 12:
		return BELL;
	}
	return KEY;
}

vector <BonusItems> Level::getFruitsAtBottomRight() const {
	vector <BonusItems> res;
	int from = (currentLevel <= 19) ? idxFromTo[currentLevel - 1].first : 12;
	int to = (currentLevel <= 19) ? idxFromTo[currentLevel - 1].second : 18;

	for (int i = from; i <= to; i++)
		res.push_back(fruitAtBRScreen[i]);
	return res;
}

pair<int, int> Level::getDrawFruitOnScreenIdxFromTo() const {
	return currentLevel <= 7 ? make_pair(0, static_cast<int> (currentLevel) - 1) : make_pair(0, 6);
}
