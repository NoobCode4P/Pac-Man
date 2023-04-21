#include "ImageButton.h"

ImageButton::ImageButton(Vector2f pos, Vector2f size, const vector <Texture>* textures, Color activeColor, Color hoverColor, Color selectedColor, bool BounceBack, bool isRadioType)
	: Button(pos, size, activeColor, hoverColor, selectedColor, BounceBack, isRadioType), animator(&buttonShape) {
		
	buttonShape.setTexture(&textures->operator[](0));

	buttonShape.setOutlineThickness(1.5f);
	
	animator.setShortClip(const_cast <vector <Texture>*> (textures));
}

void ImageButton::updateButton(const Vector2f& mousePos, const Event& mouseEvent, float deltaTime) {
	Button::updateButton(mousePos, mouseEvent);

	switch (state) {
	case ACTIVE:
		break;
	case HOVER:
		animator.update(deltaTime);
		break;
	case SELECTED:
		break;
	}
}