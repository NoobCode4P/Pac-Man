#pragma once
#include "../Game/Game.h"
#include "../GUI/Button/ImageButton.h"
#include "../GUI/Button/TextButton.h"

typedef map <string, Button*>::iterator ButtonMapIterator;

template <class Class, typename PROCEDURE>
inline void mapStructureTraversal(map <string, Class*> mp, PROCEDURE f) {
	for (class map <string, Class*>::iterator iter = mp.begin(); iter != mp.end(); iter++)
		f(iter);
}

enum GAMESTATES {
	DISCLAIMER, MAINMENU, PLAY, PRACTICE, LEARNGHOSTCHASE, CUTSCENES, COFFEEBREAK1, COFFEEBREAK2, COFFEEBREAK3
};

class GameStatesManager;

class GameState {
protected:
	bool QUIT_STATE;

	Font textFont;
	Sprite pacmanTitle_S;

	Text creditText;

public:
	GameState();
	virtual ~GameState();
	virtual void startMode();
	virtual void updateState(float deltaTime, RenderWindow* window, const Event& event) = 0;
	virtual void drawState(RenderWindow* window);
	bool isStateEnded() const;
};