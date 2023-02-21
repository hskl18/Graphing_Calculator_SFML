#ifndef CONFIG_H
#define CONFIG_H
#include <SFML/Graphics.hpp>
#include <unordered_map>
using namespace std;


enum TEXTURES
{
    BACK_GROUND = 0,
    ERROR_IMAGE = 1,
};

enum FONTS
{
    ARIAL = 0,
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
const sf::Vector2f INPUT_BOX_SIZE = sf::Vector2f(300, 100);
const sf::Vector2f INPUT_BOX_POS = sf::Vector2f(680, 840);
const sf::Vector2f ERROR_IMAGE_POS = sf::Vector2f(300, 300);
const sf::Vector2f X_AXIS_POS = sf::Vector2f(0, 500);
const sf::Vector2f Y_AXIS_POS = sf::Vector2f(600, 0);
const sf::Vector2f HISTORY_BAR_SIZE = sf::Vector2f(200, 1200);
const sf::Vector2f HISTORY_BAR_POS = sf::Vector2f(1000, 0);

class Config
{
private:
    std::unordered_map<int, std::shared_ptr<sf::Texture>> _texture_map;
    std::unordered_map<int, std::shared_ptr<sf::Font>> _font_map;
    std::shared_ptr<sf::Font> _get_font(int key);
    std::shared_ptr<sf::Texture> _get_texture(int key);
    void _set_texture();
    void _set_font();
public:
    Config();
    ~Config();
    sf::Texture& get_texture(int index);
    sf::Font& get_font(int index);
};



#endif