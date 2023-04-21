#pragma once
#include "../Common.h"

enum Directions {
	UP, LEFT, DOWN, RIGHT, NONE
};

Directions getOppositeDirection(const Directions& direction);

struct Node {
	Vector2i gridPos;
	vector <Node*> neighbors;
	set <Directions> discardedDirections;

	int g;		// Exact cost of the path from source
	int h;		// Heuristic estimated cost

	bool isInOpenList;
	bool isInClosedList;

	Vector2i parentGridPos;

	bool isVisited;

	Node(Vector2i gridPos) : gridPos(gridPos), parentGridPos(gridPos) {
		for (size_t i = 0; i < 4; i++)
			neighbors.push_back(NULL);
		discardedDirections.insert(NONE);
		g = INT_MAX;
		h = 0;
		isInOpenList = isInClosedList = false;
		isVisited = false;
	}

	~Node() {}

	int f() const { return g + h; }
};