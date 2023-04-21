#include "Maze.h"

Maze Maze::instance;

Maze::Maze() {
	mazePath = NULL;
	loadMazeSprite();
	loadMazePattern();
	if (!mazePattern.empty())
		mazePath = new MazePath(mazePattern);
}

Maze::~Maze() {
	if (mazePath)
		delete mazePath;
}

void Maze::loadMazeSprite() {
	mazeSprite.setTexture(*GameTextures::getInstance().getMazeTexture());
	mazeSprite.setScale(COMMON_SCALE, COMMON_SCALE);
	mazeSprite.move(MAZE_TOPLEFT_COORDS.x, MAZE_TOPLEFT_COORDS.y);
}

void Maze::loadMazePattern() {
	ifstream in("Maze/MazePattern.txt");
	if (!in) {
		cout << "Unable to open MazePattern.txt" << endl;
		return;
	}
	for (size_t i = 0; i < NUMBER_OF_ROWS; i++) {
		string line = "";
		for (size_t j = 0; j < NUMBER_OF_COLUMNS; j++) {
			char c;
			in >> c;
			line += c;
		}
		mazePattern.push_back(line);
	}
	in.close();
}

bool Maze::isPairOfPortals(const pair <Vector2i, Vector2i>& portal) const {
	Vector2i p1 = portal.first, p2 = portal.second;
	return (p1.y == p2.y && p1.y == 17) && ((p1.x == 0 && p2.x == NUMBER_OF_COLUMNS - 1) || (p2.x == 0 && p1.x == NUMBER_OF_COLUMNS - 1));
}

bool Maze::isPortal(const Vector2i& pos) const {
	return (pos.y == 17 && pos.x == NUMBER_OF_COLUMNS - 1) || (pos.y == 17 && pos.x == 0);
}

Vector2f Maze::getCenterOfTile(const Vector2i& gridPos) const {
	return Vector2f(gridPos.x * TILESIZE * COMMON_SCALE + TILESIZE * COMMON_SCALE / 2, gridPos.y * TILESIZE * COMMON_SCALE + TILESIZE * COMMON_SCALE / 2);
}

float Maze::getTopCoordOfTile(const Vector2i& gridPos) const {
	return gridPos.y * TILESIZE * COMMON_SCALE;
}

float Maze::getBottomCoordOfTile(const Vector2i& gridPos) const {
	return (gridPos.y + 1) * TILESIZE * COMMON_SCALE - 1;
}

float Maze::getLeftCoordOfTile(const Vector2i& gridPos) const {
	return gridPos.x * TILESIZE * COMMON_SCALE;
}

float Maze::getRightCoordOfTile(const Vector2i& gridPos) const {
	return (gridPos.x + 1) * TILESIZE * COMMON_SCALE - 1;
}

pair <vector <Vector2i>, vector <Vector2i>> Maze::pelletsLocations() const {
	vector <Vector2i> pellets, powerPellets;
	for (size_t i = 0; i < mazePattern.size(); i++)
		for (size_t j = 0; j < mazePattern[0].size(); j++) {
			if (mazePattern[i][j] == '.' || mazePattern[i][j] == '+')
				pellets.push_back(Vector2i(j, i));
			else if (mazePattern[i][j] == 'P')
				powerPellets.push_back(Vector2i(j, i));
		}
	return make_pair(pellets, powerPellets);
}

vector <string> Maze::getMazePattern() const {
	return mazePattern;
}

bool Maze::isInMaze(const Vector2i& pos) const {
	return (0 <= pos.x && pos.x < NUMBER_OF_COLUMNS) && (0 <= pos.y && pos.y < NUMBER_OF_ROWS);
}

bool Maze::isWalkableForPacMan(const Vector2i& pos) const {
	return mazePath->isWalkable(pos) && !isInHouse(pos);
}

bool Maze::isInTunnel(const Vector2i& pos) const {
	return (pos.y == 17 && ((0 <= pos.x && pos.x <= 5) || (22 <= pos.x && pos.x < NUMBER_OF_COLUMNS)));
}

bool Maze::isInHouse(const Vector2i& pos) const {
	return (11 <= pos.x && pos.x <= 16) && (15 <= pos.y && pos.y <= 18);
}

MazePath* Maze::getMazePath() const {
	return mazePath;
}

Maze& Maze::getInstance(){
	return instance;
}

void Maze::drawMaze(RenderWindow* window) {
	window->draw(mazeSprite);
}