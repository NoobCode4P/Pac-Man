#include "MazePath.h"

Directions getOppositeDirection(const Directions& direction) {
	if (direction == UP)
		return DOWN;
	else if (direction == LEFT)
		return RIGHT;
	else if (direction == DOWN)
		return UP;
	else if (direction == RIGHT)
		return LEFT;
	return NONE;
}

MazePath::MazePath(vector <string> mazePattern) {
	int nRow = mazePattern.size();
	int nCol = mazePattern[0].length();

	maze.resize(nRow, vector <Node*>(nCol));

	for (int i = 0; i < nRow; i++) {
		for (int j = 0; j < nCol; j++) {
			if (mazePattern[i][j] == 'X' || mazePattern[i][j] == '#')
				maze[i][j] = NULL;
			else
				maze[i][j] = new Node(Vector2i(j, i));
		}
	}

	// CONNECT NODES
	for (int i = 0; i < nRow; i++) {
		for (int j = 0; j < nCol; j++) {
			if (mazePattern[i][j] != 'X' && mazePattern[i][j] != '#') {
				if (i - 1 >= 0 || maze[nRow - 1][j])
					maze[i][j]->neighbors[UP] = (i - 1 >= 0) ? maze[i - 1][j] : maze[nRow - 1][j];
				else
					maze[i][j]->neighbors[UP] = NULL;

				if (j - 1 >= 0 || maze[i][nCol - 1])
					maze[i][j]->neighbors[LEFT] = (j - 1 >= 0) ? maze[i][j - 1] : maze[i][nCol - 1];
				else
					maze[i][j]->neighbors[LEFT] = NULL;

				if (i + 1 < nRow || maze[0][j])
					maze[i][j]->neighbors[DOWN] = (i + 1 < nRow) ? maze[i + 1][j] : maze[0][j];
				else
					maze[i][j]->neighbors[DOWN] = NULL;

				if (j + 1 < nCol || maze[i][0])
					maze[i][j]->neighbors[RIGHT] = (j + 1 < nCol) ? maze[i][j + 1] : maze[i][0];
				else
					maze[i][j]->neighbors[RIGHT] = NULL;
			}
		}
	}

	
}

MazePath::~MazePath() {
	for (int i = 0; i < maze.size(); i++)
		for (int j = 0; j < maze[i].size(); j++)
			if (maze[i][j]) {
				delete maze[i][j];
				maze[i][j] = NULL;
			}
}

void MazePath::resetMazeData() {
	for (int i = 0; i < maze.size(); i++) {
		for (int j = 0; j < maze[i].size(); j++) {
			if (maze[i][j]) {
				maze[i][j]->g = INT_MAX;
				maze[i][j]->h = 0;
				maze[i][j]->discardedDirections.clear();
				maze[i][j]->isInClosedList = maze[i][j]->isInOpenList = false;
				maze[i][j]->parentGridPos = maze[i][j]->gridPos;
				maze[i][j]->isVisited = false;
			}
		}
	}
}

bool MazePath::isWalkable(const Vector2i& pos) const {
	return maze[pos.y][pos.x] != NULL;
}

bool MazePath::isRedZoneRestriction(const Vector2i& gridPos) const {
	return (gridPos == Vector2i(12, 14) || gridPos == Vector2i(15, 14) || gridPos == Vector2i(12, 26) || gridPos == Vector2i(15, 26));
}

bool MazePath::isHomeDoor(const Vector2i& gridPos) const {
	return gridPos == Vector2i(13, 15) || gridPos == Vector2i(14, 15);
}

bool MazePath::isPairOfPortals(const Vector2i& p1, const Vector2i& p2){
	return (p1.y == p2.y && p1.y == 17) && ((p1.x == 0 && p2.x == NUMBER_OF_COLUMNS - 1) || (p2.x == 0 && p1.x == NUMBER_OF_COLUMNS - 1));
}

vector <Vector2i> MazePath::findPath(const Vector2i& src, const Vector2i& dest, const Directions& curDir, bool isRedZoneRestriction, bool isHomeAccessDenied){
	//=========Reset Nodes'data===========
	resetMazeData();

	//================A * algorithm=================
	struct cmp {
		bool operator()(const Node* a, const Node* b) {
			return a->f() > b->f();
		}
	};

	auto ManhattanDistance = [&dest](const Node* a) {
		return 1 * (abs(a->gridPos.x - dest.x) + abs(a->gridPos.y - dest.y));
	};

	priority_queue <Node*, vector <Node*>, cmp> openList;
	
	Node* srcNode = maze[src.y][src.x];
	Node* destNode = maze[dest.y][dest.x];

	bool foundPath = false;

	srcNode->isInOpenList = true;
	srcNode->g = 0;
	srcNode->discardedDirections.insert(getOppositeDirection(curDir));
	if (isRedZoneRestriction && this->isRedZoneRestriction(srcNode->gridPos))
		srcNode->discardedDirections.insert(UP);
	openList.push(srcNode);

	Node* currentNode = NULL;

	while (!openList.empty()) {
		currentNode = openList.top();
		openList.pop();
		currentNode->isInClosedList = true;
		
		if (destNode && currentNode->gridPos == destNode->gridPos) {
			foundPath = true;
			break;
		}
		for (int i = 0; i < currentNode->neighbors.size(); i++) {
			Node* neighbor = (currentNode->discardedDirections.find(static_cast <Directions> (i)) == currentNode->discardedDirections.end()) ? currentNode->neighbors[i] : NULL;

			if (neighbor && !neighbor->isInClosedList) {
				if (currentNode->g < INT_MAX) {
					int newGCost = currentNode->g + 1;
					if (newGCost < neighbor->g) {
						neighbor->g = newGCost;
						neighbor->h = ManhattanDistance(neighbor);
						neighbor->discardedDirections.insert(getOppositeDirection(static_cast <Directions> (i)));
						neighbor->parentGridPos = Vector2i(currentNode->gridPos.x, currentNode->gridPos.y);

						if (!neighbor->isInOpenList) {
							if (isRedZoneRestriction && this->isRedZoneRestriction(neighbor->gridPos))
								neighbor->discardedDirections.insert(UP);
							if (!isHomeAccessDenied || (isHomeAccessDenied && !isHomeDoor(neighbor->gridPos))) {
								neighbor->isInOpenList = true;
								openList.push(neighbor);
							}
						}
					}
				}
			}
		}
		
	}
	
	vector <Vector2i> path;
	if (foundPath) {
		stack <Vector2i> tmp;
		while (currentNode->gridPos != srcNode->gridPos) {
			tmp.push(currentNode->gridPos);
			currentNode = maze[currentNode->parentGridPos.y][currentNode->parentGridPos.x];
		}
		path.push_back(srcNode->gridPos);
		while (!tmp.empty()) {
			path.push_back(tmp.top());
			tmp.pop();
		}
	}

	return path;
}

vector <Vector2i> MazePath::findPathToNearAccessibleTile(const Vector2i& src, const Vector2i& dest, const Directions& currentDirection, bool isRedZoneRestriction, bool isHomeAccessDenied, int numberOfPositions) {
	vector <Vector2i> path;
	
	path.push_back(src);
	
	Directions curDir = currentDirection;

	auto getNextDirection = [this](const Vector2i& p1, const Vector2i& p2) -> Directions {
		if (p1.x == p2.x) {
			if (p1.y > p2.y)
				return UP;
			else
				return DOWN;
		}
		else if (p1.y == p2.y) {
			if (isPairOfPortals(p1, p2)) {
				if (p1.x > p2.x)
					return RIGHT;
				else
					return LEFT;
			}
			else {
				if (p1.x > p2.x)
					return LEFT;
				else
					return RIGHT;
			}
		}
		return NONE;
	};

	while (path.size() < numberOfPositions && (path.size() == 1 || path.back() != dest)) {
		Vector2i pos = getNearestAccessibleAdjacentPosition(path.back(), dest, curDir, isRedZoneRestriction, isHomeAccessDenied);
		curDir = getNextDirection(path.back(), pos);
		path.push_back(pos);
	}
	
	return path;
}

Vector2i MazePath::getNearestAccessibleAdjacentPosition(const Vector2i& src, const Vector2i& dest, const Directions& curDir, bool isRedZoneRestriction, bool isHomeAccessDenied) {
	resetMazeData();

	Node* currentNode = maze[src.y][src.x];

	auto isValidDirection = [&currentNode, isHomeAccessDenied, this](Directions dir) -> bool {
		return (currentNode->neighbors[dir] && (isHomeAccessDenied == false || (isHomeAccessDenied == true && !this->isHomeDoor(currentNode->neighbors[dir]->gridPos))));
	};

	auto distanceFrom = [dest](const Vector2i& pos) -> double {
		return sqrt(pow(dest.x - pos.x, 2) + pow(dest.y - pos.y, 2));
	};

	vector <Directions> validDirections;
	for (int i = 0; i < currentNode->neighbors.size(); i++) {
		Directions dir = static_cast <Directions> (i);
		if (isRedZoneRestriction && this->isRedZoneRestriction(currentNode->gridPos) && dir == UP)
			continue;

		if (isValidDirection(dir) && dir != getOppositeDirection(curDir)) {
			validDirections.push_back(dir);
		}
	}
	if (validDirections.empty()) {	//Dead-end => turn back
		validDirections.push_back(getOppositeDirection(curDir));
	}

	float minDistance = FLT_MAX;
	Vector2i posOfMin;
	for (Directions direction : validDirections) {
		if (minDistance > distanceFrom(currentNode->neighbors[direction]->gridPos)) {
			posOfMin = currentNode->neighbors[direction]->gridPos;
			minDistance = distanceFrom(currentNode->neighbors[direction]->gridPos);
		}
	}
	return posOfMin;
}

Vector2i MazePath::getRandomAccessibleAdjacentPosition(const Vector2i& src, const Directions& curDir) const {
	Directions oppositeDir = getOppositeDirection(curDir);
	Node* currentNode = maze[src.y][src.x];
	vector <Directions> validDirections;
	if (currentNode) {
		for (size_t i = 0; i < currentNode->neighbors.size(); i++) {
			if (currentNode->neighbors[i] && static_cast <Directions> (i) != oppositeDir)
				validDirections.push_back(static_cast <Directions> (i));
		}
	}
	
	if (validDirections.empty())
		validDirections.push_back(oppositeDir);

	switch (validDirections[rand() % validDirections.size()]) {
	case UP:
		return Vector2i(src.x, src.y - 1);
	case LEFT:
		return Vector2i(src.x - 1, src.y);
	case DOWN:
		return Vector2i(src.x, src.y + 1);
	case RIGHT:
		return Vector2i(src.x + 1, src.y);
	}
	// STANDSTILL DUE TO 'NONE' DIRECTION
	return src;
}
