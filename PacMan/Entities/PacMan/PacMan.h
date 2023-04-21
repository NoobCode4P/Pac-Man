#pragma once
#include "../Entity.h"

class PacMan : public Entity {
private:
	bool isGridPosUpdated;

	bool state;
	bool isPoweredUp;
	
	Directions inputDirection;
	Vector2i newGridPos;

	unordered_set <pair <int, int>, HashFunctionForPairII>* pelletLocations, *powerPelletLocations, *fruitLocations;

	//=========Utility functions==========
	void loadDirectionalTexture();

	Directions getUserInput();
	void pickNextTile();
	void updateAnimation();
	void Move(float deltaTime = 0.5);

	PacMan();
	static PacMan instance;
public:
	PacMan& operator = (const PacMan&) = delete;
	pair <int,int> eatenItemPos;

	//==========Boolean variables===========
	bool isEatingGhost;
	bool isEatingPellet, isEatingPowerPellet, isEatingFruit;
	bool isMoving;
	bool isInvincible;

	//==========Getters============
	static PacMan& getInstance();

	Directions getCurrentDirection() const;
	const Texture* getTexture() const;
	Vector2f getPosition() const;

	//==========Setters============
	void setMainTexture();
	void setPelletsLocation(unordered_set <pair<int, int>, HashFunctionForPairII>& pelletLocations);
	void setBonusItemLocations(vector <unordered_set <pair <int, int>, HashFunctionForPairII>*> bonusItemLocations);
	void setInvincible(bool isInvincible);

	//=========Managing PacMan's states===========
	bool isAlive() const;
	bool isEnergized() const;
	void becomeEnergized();
	void eatGhost();
	void finishEatingGhost();

	//==========Placement=============
	void restartAtLocation(Vector2i position);
	void moveToOriginalStartingPosition();
	
	//=========Other methods=========
	void update(float deltaTime = 0.5);
	void draw(RenderWindow* window);
	
	void die();
	void revive();
};