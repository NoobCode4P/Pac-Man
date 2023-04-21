#include "GraphicsPath.h"

GraphicsPath::GraphicsPath(Color color) : color(color) {
	thickness = 2;
	currentIndexInPath = 0;
	path = NULL;
	
}

GraphicsPath::GraphicsPath() {
	thickness = 2;
	currentIndexInPath = 0;
	path = NULL;
	
	arrowHead.setPointCount(3);
	const float edgeLength = 15;
	float heightOfArrowHead = sqrt(3) / 2 * edgeLength;
	
	upHead.setPointCount(3);
	upHead.setPoint(0, Vector2f(0, 0));
	upHead.setPoint(1, Vector2f(-edgeLength / 2, heightOfArrowHead));
	upHead.setPoint(2, Vector2f(edgeLength / 2, heightOfArrowHead));

	downHead.setPointCount(3);
	downHead.setPoint(0, Vector2f(edgeLength / 2, heightOfArrowHead));
	downHead.setPoint(1, Vector2f(0, 0));
	downHead.setPoint(2, Vector2f(edgeLength, 0));
	downHead.setOrigin(Vector2f(edgeLength / 2, heightOfArrowHead));

	leftHead.setPointCount(3);
	leftHead.setPoint(0, Vector2f(0,0));
	leftHead.setPoint(1, Vector2f(heightOfArrowHead, -edgeLength / 2));
	leftHead.setPoint(2, Vector2f(heightOfArrowHead, edgeLength / 2));
	
	rightHead.setPointCount(3);
	rightHead.setPoint(0, Vector2f(0, 0));
	rightHead.setPoint(1, Vector2f(-heightOfArrowHead, -edgeLength / 2));
	rightHead.setPoint(2, Vector2f(-heightOfArrowHead, edgeLength / 2));
}

void GraphicsPath::drawPath(RenderWindow* window) {
	for (size_t i = currentIndexInPath; i < lineSegments.size(); i++)
		window->draw(lineSegments[i]);
	if (!lineSegments.empty())
		window->draw(arrowHead);
}

void GraphicsPath::setColor(Color color) {
	this->color = color;
	
	upHead.setFillColor(color);
	downHead.setFillColor(color);
	leftHead.setFillColor(color);
	rightHead.setFillColor(color);
}

void GraphicsPath::setPath(const vector <Vector2i>& path) {
	this->path = &path;
}

void GraphicsPath::clearPath() {
	if (!lineSegments.empty())
		lineSegments.clear();
}

void GraphicsPath::setCurrentIndex(int currentIdx) {
	currentIndexInPath = currentIdx;
}

void GraphicsPath::renderPath() {
	if (path->size() < 2)
		return;

	auto checkConnection = [](const Vector2i& p1, const Vector2i& p2)->short {
		if (p1.x == p2.x)		// On the same column
			return 1;
		else if (p1.y == p2.y)	// On the same row
			return 2;
		return -1;
	};
	if (!lineSegments.empty())
		lineSegments.clear();

	for (size_t i = 1; i < path->size(); i++) {
		if (Maze::getInstance().isPairOfPortals(make_pair(path->operator[](i), path->operator[](i - 1)))) {
			continue;
		}
		RectangleShape segment;
		Vector2f center1 = Maze::getInstance().getCenterOfTile(path->operator[](i));
		Vector2f center2 = Maze::getInstance().getCenterOfTile(path->operator[](i - 1));
		float length = 0;
		short check = checkConnection(path->operator[](i - 1), path->operator[](i));
		if (check == 1) {
			length = abs(center1.y - center2.y);
			segment.setSize(Vector2f(thickness, length));
			
			if (center1.y < center2.y) { // move up
				segment.setOrigin(segment.getLocalBounds().width / 2, segment.getLocalBounds().height);
			}
			else { // move down
				segment.setOrigin(segment.getLocalBounds().width / 2, 0);
			}
		}
		else if (check == 2) {
			length = abs(center1.x - center2.x);
			segment.setSize(Vector2f(length, thickness));
			
			if (center1.x < center2.x) { // move left
				segment.setOrigin(segment.getLocalBounds().width, segment.getLocalBounds().height / 2);
			}
			else { // move right
				segment.setOrigin(0, segment.getLocalBounds().height / 2);
			}
		}
		segment.setPosition(center2);
		segment.setFillColor(color);

		lineSegments.push_back(segment);
	}

	auto getArrowDirection = [this]()->Directions {
		Vector2i p1 = path->at(path->size() - 2), p2 = path->back();
		if (p1.x == p2.x) {
			if (p1.y > p2.y)
				return UP;
			else
				return DOWN;
		}
		else if (p1.y == p2.y) {
			if (p1.x > p2.x)
				return LEFT;
			else
				return RIGHT;
		}
		return NONE;
	};
	
	switch (getArrowDirection()) {
	case UP: 
		arrowHead = upHead;
		break;
	case LEFT:
		arrowHead = leftHead;
		break;
	case DOWN:
		arrowHead = downHead;
		break;
	case RIGHT:
		arrowHead = rightHead;
		break;
	default :
		arrowHead = downHead;
	}
	arrowHead.setPosition(Maze::getInstance().getCenterOfTile(path->back()));
	
}