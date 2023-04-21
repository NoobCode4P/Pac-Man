#include "BonusItem.h"

map <BonusItems, uint32_t> BonusItem::points = {
	{PELLET, 10},
	{POWER_PELLET, 50},
	{CHERRY, 100},
	{STRAWBERRY, 300},
	{ORANGE, 500},
	{APPLE, 700},
	{MELON, 1000},
	{GALAXIAN_FLAGSHIP, 2000},
	{BELL, 3000},
	{KEY, 5000}
};

BonusItem::BonusItem(Vector2i gridPos, BonusItems type) {
	this->gridPos = gridPos;
	this->type = type;

	loadSprites();
		
	if (type == POWER_PELLET || type == PELLET) {
		mainSprite.setPosition(Maze::getInstance().getCenterOfTile(gridPos));
		if (type == POWER_PELLET) {
			animation.setShortClip(GameTextures::getInstance().getBonusItemsTexture(type));
			animation.setSwitchTime(0.2f);
		}
	}
	else {
		loadPointsSprite();
		Vector2f middleMazePos = Vector2f(Maze::getInstance().getLeftCoordOfTile(Vector2i(14, 20)), Maze::getInstance().getCenterOfTile(Vector2i(14, 20)).y);
		mainSprite.setPosition(middleMazePos);
		pointsSprite.setPosition(middleMazePos);
	}
}

void BonusItem::resetTexture() {
	if (type == POWER_PELLET)
		mainSprite.setTexture(GameTextures::getInstance().getBonusItemsTexture(type)->operator[](0));
}

void BonusItem::loadSprites() {
	mainSprite.setTexture(GameTextures::getInstance().getBonusItemsTexture(type)->operator[](0));
	mainSprite.setOrigin(mainSprite.getLocalBounds().width / 2, mainSprite.getLocalBounds().height / 2);
	mainSprite.setScale(COMMON_SCALE, COMMON_SCALE);
}

void BonusItem::loadPointsSprite() {
	pointsSprite.setTexture(*GameTextures::getInstance().getFruitPointsTexture(type));
	pointsSprite.setOrigin(pointsSprite.getLocalBounds().width / 2, pointsSprite.getLocalBounds().height / 2);
	pointsSprite.setScale(2, 2);
}

uint32_t BonusItem::getBonusPoints() const {
	return points[type];
}

Sprite BonusItem::getPointsSprite() const {
	return pointsSprite;
}

void BonusItem::draw(RenderWindow* window) {
	window->draw(mainSprite);
}

void BonusItem::printPoints(RenderWindow* window) {
	if (type == PELLET || type == POWER_PELLET)
		return;
	window->draw(pointsSprite);
}

void BonusItem::update(const float& deltaTime) {
	if (type == POWER_PELLET)
		animation.update(deltaTime);
}