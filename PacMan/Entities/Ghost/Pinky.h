#pragma once
#include "Ghost.h"

class Pinky : public Ghost {
	void loadDirectionalTexture();
	void drawTargetingSystem(RenderWindow* window);
public:
	Pinky(Vector2i gridPos, bool applyStatesInterchange = true);
	Pinky();
	~Pinky();
	Vector2i getScatterTargetPosition() const;
	Vector2i getChaseTargetPosition() const;
	Vector2i getRevivePosition() const;
	void moveToOriginalStartingPosition();
};