#pragma once
#include "MazePath.h"
#include "../GameTextures/GameTextures.h"
#include <fstream>

class Maze {
    Sprite mazeSprite;
    
    vector <string> mazePattern;
    MazePath* mazePath;

    void loadMazeSprite();
    void loadMazePattern();

    static Maze instance;

public:
    Maze();
    ~Maze();
    vector <string> getMazePattern() const;
    Vector2f getCenterOfTile(const Vector2i& gridPos) const;
    float getTopCoordOfTile(const Vector2i& gridPos) const;
    float getBottomCoordOfTile(const Vector2i& gridPos) const;
    float getLeftCoordOfTile(const Vector2i& gridPos) const;
    float getRightCoordOfTile(const Vector2i& gridPos) const;

    pair <vector <Vector2i>, vector <Vector2i>> pelletsLocations() const;

    bool isInMaze(const Vector2i& gridPos) const;
    bool isWalkableForPacMan(const Vector2i& gridPos) const;
    bool isPairOfPortals(const pair <Vector2i, Vector2i>& portal) const;
    bool isPortal(const Vector2i& gridPos) const;
    bool isInTunnel(const Vector2i& gridPos) const;
    bool isInHouse(const Vector2i& gridPos) const;

    MazePath* getMazePath() const;

    static Maze& getInstance();

    void drawMaze(RenderWindow* window);
};