#pragma once
#include "../Button/TextButton.h"

class ContextMenu{
	RectangleShape background;
	RectangleShape box;
	
	map <string, TextButton*> options;

	Text title;
	Font* font;

public:
	ContextMenu(RenderWindow* window, Font* font, const vector <string>& listOfOptions, string title);
	~ContextMenu();
	void update(const Vector2f& mousePosition, const Event& mouseEvent);
	map <string, TextButton*>* getOptionList();
	void drawOptions(RenderWindow* window);
};