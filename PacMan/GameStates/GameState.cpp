#include "GameState.h"

GameState::GameState() {
	QUIT_STATE = false;

	pacmanTitle_S.setTexture(*GameTextures::getInstance().getPacManTitleTexture());
	pacmanTitle_S.setScale(0.08f, 0.08f);
	pacmanTitle_S.setOrigin(pacmanTitle_S.getLocalBounds().width / 2, pacmanTitle_S.getLocalBounds().height / 2);
	pacmanTitle_S.setPosition(Maze::getInstance().getCenterOfTile(Vector2i(39, 4)));

	textFont.loadFromFile("Resources/Font/PixelGameFont.ttf");

	creditText.setString("Remade by THANG NGO");
	creditText.setFont(textFont);
	creditText.setCharacterSize(14);
	creditText.setFillColor(Color::White);
	creditText.setLetterSpacing(3);
	creditText.setOrigin(creditText.getLocalBounds().width / 2, creditText.getLocalBounds().height / 2);

}

GameState::~GameState() {}

void GameState::startMode() {
	while (SoundEffect::getManager().isPlaying(SOUND_CREDIT));
}

bool GameState::isStateEnded() const {
	return QUIT_STATE;
}

void GameState::drawState(RenderWindow* window) {
	window->draw(pacmanTitle_S);
	window->draw(creditText);
}