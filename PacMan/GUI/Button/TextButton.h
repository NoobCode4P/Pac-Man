#pragma once
#include "Button.h"

class TextButton : public Button{
	Font* font;
	Text text;
public:
	TextButton(Vector2f positionOfCenter, Vector2f size, Font* font, string text, Color activeColor, Color hoverColor, Color selectedColor, bool BounceBack = false, bool isRadioType = false, short characterSize = 18);
	void updateButton(const Vector2f& mousePosition, const Event& mouseEvent, float deltaTime = 0);
	void drawButton(RenderWindow* window);
};