#pragma once
#include "Maze.h"

class GraphicsPath {
	const vector <Vector2i> *path;
	vector <RectangleShape> lineSegments;

	ConvexShape arrowHead;
	ConvexShape upHead, downHead, leftHead, rightHead;

	int currentIndexInPath;
	Color color;
	short thickness;
public:
	GraphicsPath(Color color);
	GraphicsPath();
	void setColor(Color color);
	void setPath(const vector <Vector2i>& path);
	void setCurrentIndex(int currentIndex);
	void renderPath();
	void clearPath();
	void drawPath(RenderWindow* window);
};