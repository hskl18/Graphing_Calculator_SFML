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
        this->_title.setPosition({1025, 25});
        this->_title.setCharacterSize(35);
        this->_title.setFillColor(sf::Color::Blue);
        this->_title.setString("History: ");

        this->_bar.setSize(HISTORY_BAR_SIZE);
        this->_bar.setPosition(HISTORY_BAR_POS);
        this->_buttons = vector<Button>();
        this->_bar.setPosition(HISTORY_BAR_POS);
        this->_bar.setSize(HISTORY_BAR_SIZE);
        this->_bar.setFillColor(sf::Color::Magenta);                                                
        for(int i = 0; i < 5; ++i)
        {
            Button btn("Unset", {150, 100}, 20, sf::Color::White, sf::Color::Black);
            float y = 100 + 180 * i ;
            btn.setPosition({1025, y});
            this->_buttons.push_back(btn);
        }
    }
    ~HistoryBar(){}
	void update_buttons(const vector<string>& vec)
    {
        this->set_lru_to_history(vec);
    }
    void drawTo(sf::RenderWindow &window) {
        Config config;
        //vector<string> vec = {"A", "B", "C", "D", "E"};
        sf::Font font = config.get_font(ARIAL);
        window.draw(_bar);
        this->_title.setFont(font);
        window.draw(this->_title);

        //this->set_lru_to_history(vec);
        for(int i = 4; i >= 0; --i)
        {
            Button btn = this->_buttons[i];
            btn.setFont(font);
            btn.drawTo(window);
        }		
        
	}
    void isOver(sf::RenderWindow& window)
    {
        for(int i = 0; i < 5; ++i)
        {
            if(this->_buttons[i].isMouseOver(window))
            {
                this->_buttons[i].setBackColor(sf::Color::Yellow);
                return;
            }
            this->_buttons[i].setBackColor(sf::Color::White);
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
        for(int i = 0; i < this->_buttons.size(); ++i)
        {
            this->_buttons[i].setText(history[i]);
        }
    }
    sf::Text _title;
    sf::RectangleShape _bar;
    vector<Button> _buttons;
};


#endif // !HISTORY_BAR_H