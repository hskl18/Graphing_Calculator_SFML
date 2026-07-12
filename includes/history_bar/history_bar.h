#ifndef HISTORY_BAR_H
#define HISTORY_BAR_H

#include <SFML/Graphics.hpp>
#include "../button/button.h"
#include "../config/config.h"

using namespace std;

class HistoryBar
{
public:
    HistoryBar()
    {
        Config config;
        sf::Font& font = config.get_font(UI_FONT);

        this->_title.setPosition({1020, 28});
        this->_title.setCharacterSize(22);
        this->_title.setFillColor(sf::Color(226, 232, 240));
        this->_title.setString("RECENT GRAPHS");
        this->_title.setFont(font);

        this->_bar.setSize(HISTORY_BAR_SIZE);
        this->_bar.setPosition(HISTORY_BAR_POS);
        this->_buttons = vector<Button>();
        this->_bar.setPosition(HISTORY_BAR_POS);
        this->_bar.setSize(HISTORY_BAR_SIZE);
        this->_bar.setFillColor(sf::Color(22, 32, 49));
        for(int i = 0; i < 5; ++i)
        {
            Button btn("Empty", {160, 88}, 15, sf::Color(30, 43, 64), sf::Color(203, 213, 225));
            btn.setFont(font);
            float y = 92 + 148 * i ;
            btn.setPosition({1020, y});
            this->_buttons.push_back(btn);
        }
    }
    ~HistoryBar(){}
	void update_buttons(const vector<string>& vec)
    {
        this->set_lru_to_history(vec);
    }
    void drawTo(sf::RenderTarget &window) {
        window.draw(_bar);
        window.draw(this->_title);

        for(int i = 4; i >= 0; --i)
        {
            this->_buttons[i].drawTo(window);
        }		
        
	}
    void isOver(sf::RenderWindow& window)
    {
        for(int i = 0; i < 5; ++i)
        {
            const bool hovered = this->_buttons[i].isMouseOver(window);
            this->_buttons[i].setBackColor(
                hovered ? sf::Color(41, 77, 87) : sf::Color(30, 43, 64)
            );
        }
    }
    bool isClicked(sf::Event& input, sf::RenderWindow& window, int& index)
    {
        for(int i = 0; i < 5; ++i)
        {
            if(input.type == sf::Event::MouseButtonPressed && this->_buttons[i].isMouseOver(window))
            {
                index = i;
                return true;
            }
        }
        return false;
    }   
private:    
    void set_lru_to_history(const vector<string>& history)
    {
        for(std::size_t i = 0; i < this->_buttons.size(); ++i)
        {
            this->_buttons[i].setText(history[i]);
        }
    }
    sf::Text _title;
    sf::RectangleShape _bar;
    vector<Button> _buttons;
};


#endif // !HISTORY_BAR_H
