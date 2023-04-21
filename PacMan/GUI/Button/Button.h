#pragma once
#include "../../Common.h"

enum ButtonStates {
	ACTIVE, HOVER, SELECTED
};

class Button{
	bool bounceBack, isRadioType;
	bool IS_PRESSED, IS_HELD;
	bool IS_CLICKED;
protected:
	ButtonStates state;
	RectangleShape buttonShape;
	
	Color activeColor, hoverColor, selectedColor;

public:
	Button(Vector2f positionOfCenter, Vector2f ButtonSize, Color activeColor, Color hoverColor, Color selectedColor, bool BounceBack = false, bool isRadioType = false);
	bool isSelected() const;
	bool isClicked() const;
	void select(bool select);

	virtual void updateButton(const Vector2f& mousePosition, const Event& mouseEvent, float deltaTime = 0);
	virtual void drawButton(RenderWindow* window);
};