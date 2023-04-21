#include "DisclaimerScreen.h"

DisclaimerScreen::DisclaimerScreen(RenderWindow* window) {
	title.setString("DISCLAIMER");
	title.setFont(textFont);
	title.setCharacterSize(36);
	title.setLetterSpacing(2);
	title.setFillColor(Color::Yellow);
	title.setOutlineColor(Color::Red);
	title.setOutlineThickness(1.5);
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
	title.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 8)).y);

	pacmanTitle_S.setScale(0.1f, 0.1f);
	pacmanTitle_S.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 4)).y);

	const float boxWidth = (float)window->getSize().x - 100.f, boxHeight = (float)window->getSize().y * 0.58f;

	box.setSize(Vector2f(boxWidth, boxHeight));
	box.setFillColor(Color::Transparent);
	box.setOutlineColor(Color::White);
	box.setOutlineThickness(3.f);
	box.setOrigin(box.getLocalBounds().width / 2, box.getLocalBounds().height / 2);
	box.setPosition((float)window->getSize().x / 2, (float)window->getSize().y / 2 + 62);

	readMessage();

	mascot.setTexture(*GameTextures::getInstance().getPacManFigureIconTexture());
	mascot.setOrigin(mascot.getLocalBounds().width / 2, mascot.getLocalBounds().height / 2);
	mascot.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(5, 7)));

	skipButton = new TextButton(Maze::getInstance().getCenterOfTile(Vector2i(45, 33)), Vector2f(80, 50), &textFont, "SKIP", Color::White, Color::Green, Color::Yellow, true);

	creditText.setPosition((float)window->getSize().x / 2, Maze::getInstance().getCenterOfTile(Vector2i(0, 35)).y - 8);

}

DisclaimerScreen::~DisclaimerScreen() {
	delete skipButton;
}

void DisclaimerScreen::readMessage() {
	const uint32_t charSize = 20;
	const float letterSpacing = 1.5;
	const float lineMaxWidth = box.getSize().x - 80;

	const string msg = "The resources used in this game are the property of their respective owners. All rights reserved. "
		"This game was created for educational purposes only and does not intend to infringe on any copyright or intellectual property rights of the original owners. "
		"The use of these assets in this game is solely for the purpose of enhancing the developer's programming skills and no profit will be made from this game. "
		"This game fundamentally follows the original rules of the game it is based on. " 
		"Any modifications to the game mechanisms are intended to be improvements to the original one, and not intended to infringe upon any existing rights or trademarks associated with the original game. "
		"I hope that this game can provide entertainment to those who enjoy the original game, "
		"and offer a unique programming learning experience to those interested in game development.";
		
	stringstream readS(msg);
	string word;
	float curWidth = 0;

	string text = "";

	while (readS >> word) {
		Text tmp(" " + word , textFont, charSize);
		tmp.setLetterSpacing(letterSpacing);
		tmp.setStyle(Text::Bold);

		float wordWidth = tmp.getLocalBounds().width;

		if (curWidth + wordWidth < lineMaxWidth) {
			text += (" " + word);

			curWidth += wordWidth;

		}
		else {
			text += ('\n' + word);
			curWidth = wordWidth;
		}
	}
	message.setString(text);
	message.setLetterSpacing(letterSpacing);
	message.setStyle(Text::Bold);

	message.setCharacterSize(charSize);
	message.setLineSpacing(1.3f);
	message.setFont(textFont);
	message.setFillColor(Color::Yellow);
	
	message.setOrigin(message.getLocalBounds().width / 2, message.getLocalBounds().height / 2);
	message.setPosition(box.getPosition());

}

void DisclaimerScreen::updateState(float deltaTime, RenderWindow* window, const Event& event) {
	Vector2f mousePosView = window->mapPixelToCoords(Mouse::getPosition(*window));
	skipButton->updateButton(mousePosView, event, deltaTime);
	
	if (skipButton->isClicked()) {
		SoundEffect::getManager().playSound(SOUND_CREDIT, false);
		QUIT_STATE = true;
	}
}

void DisclaimerScreen::drawState(RenderWindow* window) {
	skipButton->drawButton(window);
	window->draw(message);
	window->draw(title);
	window->draw(mascot);
	window->draw(box);
	GameState::drawState(window);
}