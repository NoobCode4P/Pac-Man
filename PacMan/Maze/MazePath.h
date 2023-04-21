#pragma once
#include "Node.h"
#include <queue>

#define NUMBER_OF_ROWS 36
#define NUMBER_OF_COLUMNS 28

struct HashFunctionForPairII {
	bool operator()(const pair <int, int>& a) const {
		return a.first ^ a.second;
	}
};

class MazePath {
	vector <vector <Node*>> maze;

	void resetMazeData();
	bool isRedZoneRestriction(const Vector2i& gridPos) const;
	bool isHomeDoor(const Vector2i& gridPos) const;
	bool isPairOfPortals(const Vector2i& pos1, const Vector2i& pos2);

public:
	MazePath(vector <string> mazePattern);
	~MazePath();
	bool isWalkable(const Vector2i& pos) const;
	vector <Vector2i> findPath(const Vector2i& source, const Vector2i& destination, const Directions& currentDirection, bool isRedZoneRestriction, bool isHomeAccessDenied);
	vector <Vector2i> findPathToNearAccessibleTile(const Vector2i& source, const Vector2i& inaccessibleDestination, const Directions& currentDirection, bool isRedZoneRestriction, bool isHomeAccessDenied, int numberOfPosition = 10);
	Vector2i getNearestAccessibleAdjacentPosition(const Vector2i& source, const Vector2i& inaccessibleDestination, const Directions& currentDirection, bool isRedZoneRestriction, bool isHomeAccessDenied);
	Vector2i getRandomAccessibleAdjacentPosition(const Vector2i& source, const Directions& currentDirection) const;
};