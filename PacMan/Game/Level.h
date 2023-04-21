#pragma once
#include "../Common.h"

class Level
{
private:
	uint32_t currentLevel;

	static vector <BonusItems> fruitAtBRScreen; // fruits at the bottom right
	vector <pair <int, int>> idxFromTo;
	
public:
	Level(uint32_t level = 1);
	void setLevel(uint32_t level);
	uint32_t getCurrentLevel() const;

	float getGhostSpeedPercentage() const;
	float getGhostTunnelSpeedPercentage() const;
	float getFrightenedGhostSpeedPercentage() const;
	float getPacManSpeedPercentage() const;
	float getMaxGhostFrightenedDuration() const;
	vector <float> getWaveEffect() const;
	vector <int8_t> getDotCounterLimit() const;
	BonusItems getBonusSymbol() const;
	vector <BonusItems> getFruitsAtBottomRight() const;
	pair <int, int> getDrawFruitOnScreenIdxFromTo() const;
};