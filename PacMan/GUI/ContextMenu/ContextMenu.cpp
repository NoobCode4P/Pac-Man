#include "ContextMenu.h"

ContextMenu::ContextMenu(RenderWindow* window, Font* font, const vector <string>& listOfOptions, string Title) {
	background.setSize(Vector2f((float)window->getSize().x, (float)window->getSize().y));
	background.setFillColor(Color(20, 20, 20, 100));
	background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds(). height / 2);
	background.setPosition(Vector2f((float)window->getSize().x / 2, (float)window->getSize().y / 2));

	box.setSize(Vector2f((float)window->getSize().x / 4, (float)window->getSize().y));
	box.setFillColor(Color(20, 20, 20, 200));
	box.setOrigin(box.getLocalBounds().width / 2, box.getLocalBounds().height / 2);
	box.setPosition(background.getPosition());


	this->font = font;
	
	if (Title != "") {
		title.setFont(*font);
		title.setFillColor(Color(255, 255, 255, 200));
		title.setCharacterSize(24);
		title.setString(Title);
		title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
		title.setPosition(
			box.getPosition().x,
			box.getPosition().y - box.getSize().y / 2 + title.getLocalBounds().height
		);
	}

	const Color activeColor = Color(SILVER_COLOR), hoverColor = Color::White, selectedColor = Color::Yellow;

	int n = static_cast <int> (listOfOptions.size());
	float centerYOfMidOpt = box.getPosition().y;
	if (n & 1) {
		options[listOfOptions[n >> 1]] = new TextButton(box.getPosition(), Vector2f(160, 50), font, listOfOptions[n >> 1], activeColor, hoverColor, selectedColor, true);
		for (int i = (n >> 1) - 1; i >= 0; i--) {
			options[listOfOptions[i]] = new TextButton(Vector2f(box.getPosition().x, centerYOfMidOpt - 100 * ((n >> 1) - i)), Vector2f(160, 50), font, listOfOptions[i], activeColor, hoverColor, selectedColor, true);
			options[listOfOptions[n - 1 - i]] = new TextButton(Vector2f(box.getPosition().x, centerYOfMidOpt + 100 * ((n >> 1) - i)), Vector2f(160, 50), font, listOfOptions[n - 1 - i], activeColor, hoverColor, selectedColor, true);
		}
	}
	else {
		for (int i = (n >> 1) - 1; i >= 0; i--) {
			options[listOfOptions[i]] = new TextButton(Vector2f(box.getPosition().x, centerYOfMidOpt - 50 - 100 * ((n >> 1) - 1 - i)), Vector2f(160, 50), font, listOfOptions[i], activeColor, hoverColor, selectedColor, true);
			options[listOfOptions[n - 1 - i]] = new TextButton(Vector2f(box.getPosition().x, centerYOfMidOpt + 50 + 100 * ((n - 1 - i) - (n >> 1))), Vector2f(160, 50), font, listOfOptions[n - 1 - i], activeColor, hoverColor, selectedColor, true);
		}
	}
}

ContextMenu::~ContextMenu() {
	for (map <string, TextButton*>::iterator it = options.begin(); it != options.end(); it++)
		delete it->second;
}

void ContextMenu::update(const Vector2f& mousePos, const Event& mouseEvent) {
	for (map <string, TextButton*>::iterator it = options.begin(); it != options.end(); it++)
		it->second->updateButton(mousePos, mouseEvent);
}

map <string, TextButton*>* ContextMenu::getOptionList() {
	return &options;
}

void ContextMenu::drawOptions(RenderWindow* window) {
	window->draw(background);
	window->draw(box);
	
	for (map <string, TextButton*>::iterator it = options.begin(); it != options.end(); it++)
		it->second->drawButton(window);

	window->draw(title);
}
