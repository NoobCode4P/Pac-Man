#include "Button.h"

Button::Button(Vector2f pos, Vector2f size, Color activeColor, Color hoverColor, Color selectedColor, bool BounceBack, bool isRadioType) {
	state = ACTIVE;
	IS_PRESSED = IS_HELD = false;
	IS_CLICKED = false;

	this->bounceBack = BounceBack;
	this->isRadioType = isRadioType;

	buttonShape.setSize(size);

	buttonShape.setOrigin(buttonShape.getLocalBounds().width / 2, buttonShape.getLocalBounds().height / 2);
	buttonShape.setPosition(pos);

	buttonShape.setOutlineColor(Color(activeColor));

	this->activeColor = activeColor;
	this->hoverColor = hoverColor;
	this->selectedColor = selectedColor;

}

bool Button::isSelected() const {
	return state == SELECTED;
}

bool Button::isClicked() const {
	return	IS_CLICKED;
}

void Button::select(bool select) {
	if (select)
		state = SELECTED;
	else
		state = ACTIVE;
}

void Button::updateButton(const Vector2f& mousePos, const Event& mE, float deltaTime) {
	auto checkMouseButtonPressed = [&mE](bool isPressedState) -> bool {	// true/false for pressed/released button
		if (isPressedState)
			return mE.type == mE.MouseButtonPressed && mE.mouseButton.button == Mouse::Left;
		return mE.type == mE.MouseButtonReleased && mE.mouseButton.button == Mouse::Left;
	};

	auto doesContain = [this, mousePos]()->bool {return buttonShape.getGlobalBounds().contains(mousePos); };

	auto isClicked = [this, &checkMouseButtonPressed, &doesContain]() -> bool{
		bool click = false;
		if (checkMouseButtonPressed(true)) {
			if (doesContain() && !IS_HELD)
				IS_PRESSED = IS_HELD = true;
			else
				IS_HELD = true;
		}

		if (checkMouseButtonPressed(false)) {
			if (IS_PRESSED && doesContain())
				click = true;
			IS_PRESSED = IS_HELD = false;
		}
		
		return click;
	};

	/*
	Radio type: clicks only once (it changes to SELECTED) (can be deselected by clicking other buttons)
	Non-radio type: 
		Bounce back up:
			It is always UNSELECTED before it is clicked
			After clicked, it is SELECTED and unselected immediately
		Otherwise:
			When UNSELECTED and clicked, it becomes SELECTED
			When SELECTED and clicked, it becomes UNSELECTED
	*/
	
	if (isRadioType) {
		IS_CLICKED = isClicked();
		if (state != SELECTED && IS_CLICKED) {
			state = SELECTED;
		}
		else {
			if (state != SELECTED) {
				if (doesContain()) {
					state = HOVER;
				}
				else state = ACTIVE;
			}
		}
	}
	else {
		if (bounceBack) {
			IS_CLICKED = isClicked();
			if (IS_CLICKED) {
				state = SELECTED;
			}
			else {
				if (doesContain())
					state = HOVER;
				else
					state = ACTIVE;
			}
		}
		else {
			IS_CLICKED = isClicked();
			if (IS_CLICKED) {
				if (state == SELECTED)
					state = ACTIVE;
				else
					state = SELECTED;
			}
			else {
				if (state != SELECTED) {
					if (doesContain())
						state = HOVER;
					else
						state = ACTIVE;
				}
			}

		}
	}
		
	switch (state) {
	case ACTIVE: {
		buttonShape.setOutlineColor(activeColor);
		break;
	}
	case HOVER: {
		buttonShape.setOutlineColor(hoverColor);
		break;
	}
	case SELECTED: {
		buttonShape.setOutlineColor(selectedColor);
		break;
	}
	
	}

}

void Button::drawButton(RenderWindow* window) {
	window->draw(buttonShape);
}