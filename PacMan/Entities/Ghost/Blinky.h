#pragma once
#include "Ghost.h"

class Blinky : public Ghost {
	void loadDirectionalTexture();
public:
	Blinky(Vector2i gridPos, bool applyStatesInterchange = true);
	Blinky();
	~Blinky();
	Vector2i getScatterTargetPosition() const;
	Vector2i getChaseTargetPosition() const;
	Vector2i getRevivePosition() const;
	void moveToOriginalStartingPosition();
};