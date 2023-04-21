#include "Animation.h"

Animation::Animation(Sprite* mainSprite, float switchTime) {
	this->mainSprite = mainSprite;
	this->textures = this->previousTextures = NULL;	
	totalTime = 0;
	this->switchTime = switchTime;
	currentIndex = 0;
	rectShape = NULL;
}

Animation::Animation(RectangleShape* rectShape, float switchTime) {
	this->rectShape = rectShape;
	this->textures = this->previousTextures = NULL;
	totalTime = 0;
	this->switchTime = switchTime;
	currentIndex = 0;
	mainSprite = NULL;
}

void Animation::setSwitchTime(float time) {
	switchTime = time;
}

void Animation::setShortClip(const vector <Texture>* textureList) {
	if (previousTextures == textureList)
		return;
	previousTextures = textures;
	textures = textureList;
	totalTime = 0;
	currentIndex = 0;
}

void Animation::update(float deltaTime) {
	totalTime += deltaTime;

	if (textures) {
		if (totalTime >= switchTime) {
			totalTime -= switchTime;
			currentIndex++;

			if (currentIndex >= textures->size())
				currentIndex = 0;

			if (mainSprite)
				mainSprite->setTexture(textures->operator[](currentIndex));
			else
				rectShape->setTexture(&textures->operator[](currentIndex));
		}
	}
}