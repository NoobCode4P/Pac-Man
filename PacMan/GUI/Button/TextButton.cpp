#include "TextButton.h"

TextButton:: TextButton(Vector2f positionOfCenter, Vector2f size, Font* font, string text, Color activeColor, Color hoverColor, Color selectedColor, bool BounceBack, bool isRadioType, short characterSize)
	: Button(positionOfCenter, size, activeColor, hoverColor, selectedColor, BounceBack, isRadioType) {

	this->font = font;
	buttonShape.setFillColor(Color::Transparent);
	buttonShape.setOutlineThickness(2.f);

	this->text.setFont(*font);
	this->text.setString(text);
	this->text.setFillColor(Color::Transparent);
	this->text.setCharacterSize(characterSize);
	this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2);
	this->text.setPosition(buttonShape.getPosition().x, buttonShape.getPosition().y - 4);
}

void TextButton::updateButton(const Vector2f& mousePos, const Event& mouseEvent, float deltaTime) {
	Button::updateButton(mousePos, mouseEvent);

	switch (state) {
	case ACTIVE: {
		text.setFillColor(activeColor);
		break;
	}
	case HOVER: {
		text.setFillColor(hoverColor);
		break;
	}
	case SELECTED: {
		text.setFillColor(selectedColor);
		break;
	}
	}
}

void TextButton::drawButton(RenderWindow* window) {
	Button::drawButton(window);
	window->draw(text);
}