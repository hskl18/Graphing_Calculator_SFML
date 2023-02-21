
#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include <sstream>
#include <SFML/Graphics.hpp>
#include <iostream>
// Define keys:
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27



class InputBox {
public:
	InputBox(){}
	InputBox(int font_size,sf::Vector2f box_size, sf::Vector2f position ,sf::Color text_color, sf::Color box_color, bool sel) 
	{
		rect.setSize(box_size); //sf::Vector2f(300, 100)
		rect.setFillColor(box_color); //sf::Color::White
		rect.setPosition(position); //{680, 840}
		textbox.setCharacterSize(font_size);
		textbox.setFillColor(text_color);
		textbox.setPosition(position);
		isSelected = sel;

		// Check if the textbox is selected upon creation and display it accordingly:
		if(isSelected)
			textbox.setString("|");
		else
			textbox.setString("");
	}
	
	// sf::Text& get_text(){return this->textbox;}
	// Make sure font is passed by reference:
	void setFont(sf::Font &fonts) {
		textbox.setFont(fonts);
	}

	void setPosition(sf::Vector2f point) {
		textbox.setPosition(point);
	}

	// Set char limits:
	void setLimit(bool ToF) {
		hasLimit = ToF;
	}

	void setLimit(bool ToF, int lim) {
		hasLimit = ToF;
		limit = lim - 1;
	}

	// Change selected state:
	void setSelected(bool sel) {
		isSelected = sel;

		// If not selected, remove the '_' at the end:
		if (!sel) {
			std::string t = text.str();
			std::string newT = "";
			for (int i = 0; i < t.length(); i++) {
				newT += t[i];
			}
			textbox.setString(newT);
		}
	}

	std::string getText() {
		return text.str();
	}
	sf::Text& get_text_box(){return this->textbox;}
	void drawTo(sf::RenderWindow &window) {

		window.draw(rect);
		window.draw(textbox);
	}

	// Function for event loop:
	void typedOn(sf::Event &input) {
		if (isSelected) {
			int charTyped = input.text.unicode;

			// Only allow normal inputs:
			if (charTyped < 128) {
				if (hasLimit) {
					// If there's a limit, don't go over it:
					if (text.str().length() <= limit) {
						inputLogic(charTyped);
					}
					// But allow for char deletions:
					else if (text.str().length() > limit && charTyped == DELETE_KEY) {
						deleteLastChar();
					}
					
				}
				// If no limit exists, just run the function:
				else {
					inputLogic(charTyped);
				}
			}
		}
	}
private:
	sf::Text textbox;
	std::ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit = 0;
	sf::RectangleShape rect;

	// Delete the last character of the text:
	void deleteLastChar() {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length() - 1; i++) {
			newT += t[i];
		}
		text.str("");
		text << newT;
		textbox.setString(text.str() + "|");
		//std::cout << text.str() << std::endl;
	}

	// Get user input:
	void inputLogic(int charTyped) {
		// If the key pressed isn't delete, or the two selection keys, then append the text with the char:
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
			text << static_cast<char>(charTyped);
		}
		// If the key is delete, then delete the char:
		else if (charTyped == DELETE_KEY) {
			if (text.str().length() > 0) {
				deleteLastChar();
			}
		}
		// Set the textbox text:
		textbox.setString(text.str() + "|");
		//std::cout << text.str() << std::endl;
	}
};




#endif