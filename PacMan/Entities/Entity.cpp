#include "Entity.h"

Entity::Entity() : animation(&mainSprite) {
	currentDirection = NONE;
	visible = true;
	freezed = false;
	curLvl = NULL;
}

Entity::~Entity() {}

Vector2i Entity::getGridPos() const {
	return gridPos;
}

void Entity::teleportHorizontallyTo(const Vector2i& newPos, bool RL) {
	mainSprite.setPosition(Maze::getInstance().getCenterOfTile(newPos).x + ((RL) ? -HALFTILE : HALFTILE), Maze::getInstance().getCenterOfTile(newPos).y);
}

void Entity::setFreezed(bool isFreezed) {
	this->freezed = isFreezed;
}

bool Entity::isFreezed() const {
	return freezed;
}

void Entity::setVisible(bool isVisible) {
	this->visible = isVisible;
}

bool Entity::isVisible() const {
	return visible;
}

void Entity::setCurrentLevelData(Level* curLvl) {
	this->curLvl = curLvl;
}