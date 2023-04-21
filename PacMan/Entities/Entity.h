#pragma once
#include "../Common.h"
#include "../Maze/GraphicsPath.h"
#include "../Animation/Animation.h"
#include "../Game/Level.h"

constexpr float COMMON_SPEED = 125;

class Entity {
protected:
	Vector2i gridPos;
	Directions currentDirection;

	Sprite mainSprite;

	bool visible;
	bool freezed;

	Animation animation;

	const Level* curLvl;
	
public:
	Entity();
	~Entity();
	Vector2i getGridPos() const;
	void teleportHorizontallyTo(const Vector2i& newPos, bool isRightToLeft);
	void setFreezed(bool isFreezed = true);
	bool isFreezed() const;
	void setVisible(bool isVisible = true);
	bool isVisible() const;
	void setCurrentLevelData(Level* currentLevel);
};