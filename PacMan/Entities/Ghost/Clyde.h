#pragma once
#include "Ghost.h"

class Clyde : public Ghost {
	void loadDirectionalTexture();
	void drawTargetingSystem(RenderWindow* window);
public:
	Clyde(Vector2i gridPos, bool applyStatesInterchange = true);
	Clyde();
	~Clyde();
	Vector2i getScatterTargetPosition() const;
	Vector2i getChaseTargetPosition() const;
	Vector2i getRevivePosition() const;
	void moveOutOfTheHouse(float delta);
	void moveIntoTheHouse(float delta);
	void moveToOriginalStartingPosition();
};