#pragma once
#include "../GameState.h"

typedef map <string, Ghost*>::iterator GhostGroupMapIterator;

class LearnGhostChaseMode : public GameState{
	map <string, Ghost*> ghostGroup;
	map <string, Button*> ghostButtons;
	
	Sprite pacmanIcon;
	vector <Sprite> ghostIcons;

	Button* backButton;
	
	string currentDisplayedGhost;
public:
	LearnGhostChaseMode(RenderWindow* window);
	~LearnGhostChaseMode();
	void updateState(float deltaTime, RenderWindow* window, const Event& event);
	void drawState(RenderWindow* window);
};