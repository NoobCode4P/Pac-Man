#pragma once
#include "Button.h"
#include "../../Animation/Animation.h"

class ImageButton : public Button{
	Animation animator;
public:
	ImageButton(Vector2f positionOfCenter, Vector2f ButtonSize, const vector <Texture>* textures, Color activeColor, Color hoverColor, Color selectedColor, bool BounceBack = false, bool isRadioType = false);
	void updateButton(const Vector2f& mousePosition, const Event& mouseEvent, float deltaTime);
};