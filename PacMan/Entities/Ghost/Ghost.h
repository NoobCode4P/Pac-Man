#pragma once
#include "../Entity.h"
#include "../PacMan/PacMan.h"

enum GhostStates {
	SCATTER, CHASE, FRIGHTENED, EATEN
};

class Ghost : public Entity{
private:
	vector <Vector2i> currentPath;
	int currentIndexInPath;
	Vector2i currentTarget;

	bool isGridPosUpdated;
	bool isReversedDirection;
	bool isHomeAccessDenied;
	bool isEatenGhostInFrontOfTheGate;
	bool eatenByPacMan;
	bool showPath, showTarget;
	
	int8_t dotCounter;
	
	GhostStates state;
	bool applyStatesInterchange;

	float waveTime;
	vector <float> waveDurations;
	uint16_t currentWave;

	float frightenedTime;

	void updateAnimation();

	//=========Utility functions for movement and placement==========
	void moveAlongMazePath(float delta);
	void pickNextTile();
	void bounceInHouse(float delta);
	void procedureToGetInsideHouse(float delta);


protected:
	const vector <Texture> *upTexture, *leftTexture, *downTexture, *rightTexture;
	
	RectangleShape targetTile;

	GraphicsPath* pathSketcher;

	bool isSpawned;
	bool isOutside;

	virtual void loadDirectionalTexture() = 0;
	
	virtual void drawTargetingSystem(RenderWindow* window);

	//==================Movement=======================
	virtual void moveOutOfTheHouse(float delta);
	virtual void moveIntoTheHouse(float delta);
	
	
public:
	Ghost(Vector2i gridPos, bool applyStateInterChange = true);
	~Ghost();

	//=============Ghost's states setter and getter============
	GhostStates getState() const;
	void setState(GhostStates ghostState);

	//=============Placement=================
	virtual void moveToOriginalStartingPosition() = 0;

	//===============Target tile position Getters================
	virtual Vector2i getScatterTargetPosition() const = 0;
	virtual Vector2i getChaseTargetPosition() const = 0;
	virtual Vector2i getRevivePosition() const = 0;

	//=============Ghost's actions==================
	void becomeFrightened();
	void reverseDirection();
	void ShowPath(bool isPathShown);
	void ShowTarget(bool isTargetShown);

	//=============Dot Counter related=============
	bool activateDotCounter;
	void setDotCounter(int8_t dotCounterLimit);
	void countDots();
	bool reachesDotLimit() const;

	//=============House related=============
	bool isInsideHouse() const;
	void spawn();

	//=============EatenByPacMan setter and getter===========
	bool isEatenByPacMan() const;
	void setEatenByPacMan(bool eatenByPacMan);

	//==========Other methods===========
	Vector2f getGhostPosition() const;
	void resetData();
	void setWaveDurations(const vector <float>& WaveEffectDurations);
	const vector <Texture>& getTextures(Directions direction = DOWN) const;

	void update(float deltaTime = 0.5);
	void drawGhost(RenderWindow* window);
};