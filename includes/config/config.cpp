#include "config.h"



Config::Config()
{
    this->_set_texture();
    this->_set_font();
}

Config::~Config(){}


std::shared_ptr<sf::Texture> Config::_get_texture(int key)
{
    if(this->_texture_map.find(key) != this->_texture_map.end()) 
    {
        return this->_texture_map[key];
    }
    return nullptr;
}

std::shared_ptr<sf::Font> Config::_get_font(int key)
{
    if(this->_font_map.find(key) != this->_font_map.end()) 
    {
        return this->_font_map[key];
    }
    return nullptr;
}



sf::Texture& Config::get_texture(int index)
{
    std::shared_ptr<sf::Texture> texture = this->_get_texture(index);
    assert(texture != nullptr && "Getting non existed texture");
    return *texture;
}


sf::Font& Config::get_font(int index)
{
    std::shared_ptr<sf::Font> font = this->_get_font(index);
    assert(font != nullptr && "Getting non existed font");
    return *font;
}




void Config::_set_texture()
{
    auto t1 = std::make_shared<sf::Texture>();
    t1->loadFromFile("../assets/cat1.png");
    this->_texture_map[BACK_GROUND] = t1;
    auto t2 = std::make_shared<sf::Texture>();
    t2->loadFromFile("../assets/dog.png");
    this->_texture_map[ERROR_IMAGE] = t2;
}

void Config::_set_font()
{
    auto f1 = std::make_shared<sf::Font>();
    f1->loadFromFile("../assets/arial.ttf");
    this->_font_map[ARIAL] = f1;
}