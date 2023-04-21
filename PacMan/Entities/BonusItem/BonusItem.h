#pragma once
#include "../Entity.h"

class BonusItem : public Entity{
	static map <BonusItems, uint32_t> points;
	
	BonusItems type;

	Sprite pointsSprite;

	void loadSprites();
	void loadPointsSprite();

public:
	BonusItem(Vector2i gridPos, BonusItems type);
	void resetTexture();
	uint32_t getBonusPoints() const;
	Sprite getPointsSprite() const;
	void update(const float& deltaTime = 0.5);
	void draw(RenderWindow* window);
	void printPoints(RenderWindow* window);
};