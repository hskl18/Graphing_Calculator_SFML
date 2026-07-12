#include "button.h"

Button::Button() {

}

Button::Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
	button.setSize(buttonSize);
	button.setFillColor(bgColor);
	button.setOutlineColor(sf::Color(51, 65, 85));
	button.setOutlineThickness(1.0f);

	// Get these for later use:
	btnWidth = buttonSize.x;
	btnHeight = buttonSize.y;

	text.setString(btnText);
	text.setCharacterSize(charSize);
	text.setFillColor(textColor);
}

void Button::setText(std::string t) {
	if (t.size() > 19) {
		t = t.substr(0, 16) + "...";
	}
	text.setString(t);
	centerText();
}

void Button::setSize(sf::Vector2f s) {
	button.setSize(s);
}

void Button::setCharSize(int c) {
	text.setCharacterSize(c);
}

void Button::setFont(sf::Font &fonts) {
	text.setFont(fonts);
	centerText();
}

void Button::setBackColor(sf::Color color) {
	button.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
	text.setFillColor(color);
}

void Button::setPosition(sf::Vector2f point) {
	button.setPosition(point);
	centerText();
}

void Button::centerText() {
	const sf::FloatRect bounds = text.getLocalBounds();
	const sf::Vector2f position = button.getPosition();
	text.setPosition(
		position.x + (btnWidth - bounds.width) / 2.0f - bounds.left,
		position.y + (btnHeight - bounds.height) / 2.0f - bounds.top - 2.0f
	);
}

void Button::drawTo(sf::RenderTarget &window) {
	window.draw(button);
	window.draw(text);
}

// Check if the mouse is within the bounds of the button:
bool Button::isMouseOver(sf::RenderWindow &window) {
	sf::Vector2i mouseCoords({ sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y });

	sf::Vector2f realCoords = window.mapPixelToCoords(mouseCoords);

	float mouseX = realCoords.x;
	float mouseY = realCoords.y;

	int btnPosX = button.getPosition().x;
	int btnPosY = button.getPosition().y;

	int btnxPosWidth = button.getPosition().x + button.getGlobalBounds().width;
	int btnyPosHeight = button.getPosition().y + button.getGlobalBounds().height;

	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
		return true;
	}
	return false;
}
