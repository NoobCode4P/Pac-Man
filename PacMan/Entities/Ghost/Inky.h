#pragma once
#include "Ghost.h"

class Inky : public Ghost {
	Ghost* blinky;
	void loadDirectionalTexture();
	void drawTargetingSystem(RenderWindow* window);
public:
	Inky(Vector2i gridPos, Ghost* ghost = NULL, bool applyStatesInterchange = true);
	Inky(Ghost* ghost = NULL);
	~Inky();
	Vector2i getScatterTargetPosition() const;
	Vector2i getChaseTargetPosition() const;
	Vector2i getRevivePosition() const;
	void moveOutOfTheHouse(float delta);
	void moveIntoTheHouse(float delta);
	void moveToOriginalStartingPosition();
};