#pragma once
#include "../Common.h"

class Animation {
	float switchTime;
	float totalTime;
	int currentIndex;
	const vector <Texture>* textures, *previousTextures;
	Sprite* mainSprite;
	RectangleShape* rectShape;
public:
	Animation(Sprite* mainSprite, float switchTime = 0.1);
	Animation(RectangleShape* rectShape, float switchTime = 0.1);
	void setSwitchTime(float switchTime = 0.1);
	void setShortClip(const vector <Texture> *textureList);
	void update(float deltaTime = 0.2);
};