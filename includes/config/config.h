#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>


enum FONTS
{
    UI_FONT = 0,
};

// windows
const int WINDOW_HEIGHT = 1000;
const int WINDOW_WIDTH = 1200;
const int INPUT_BOX_FONT_SIZE = 24;
const int CHAR_LIMIT = 35;
const float ZOOM_MAX = 1.5;
const float ZOOM_MIN = 0.5;
const float ZOOM_DELTA = 0.25;

// system variable config
const sf::Vector2f INPUT_BOX_SIZE = sf::Vector2f(640, 64);
const sf::Vector2f INPUT_BOX_POS = sf::Vector2f(180, 906);
const sf::Vector2f ERROR_PANEL_SIZE = sf::Vector2f(320, 180);
const sf::Vector2f ERROR_PANEL_POS = sf::Vector2f(340, 360);
const sf::Vector2f X_AXIS_POS = sf::Vector2f(0, 500);
const sf::Vector2f Y_AXIS_POS = sf::Vector2f(500, 0);
const sf::Vector2f HISTORY_BAR_SIZE = sf::Vector2f(200, 1200);
const sf::Vector2f HISTORY_BAR_POS = sf::Vector2f(1000, 0);

class Config
{
private:
    static sf::Font& _ui_font();
public:
    Config();
    sf::Font& get_font(int index);
};



#endif
