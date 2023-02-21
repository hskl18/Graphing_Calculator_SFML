#ifndef PLOT_H
#define PLOT_H
#include "../lib/shunting_yard/shunting_yard.h"
#include "../lib/rpn/rpn.h"
#include "../config/config.h"
#include <SFML/Graphics.hpp>


struct ColorGenerator
{
    unordered_map<int, sf::Color> _color_map;
    ColorGenerator()
    {
        _color_map[0] = sf::Color::Yellow;
        _color_map[1] = sf::Color::White;
        _color_map[2] = sf::Color::Blue;
        _color_map[3] = sf::Color::Green;
        _color_map[4] = sf::Color::Cyan;
        _color_map[5] = sf::Color(255, 153, 153);
        _color_map[6] = sf::Color(255, 128, 0);
        _color_map[7] = sf::Color(102, 0, 204);
        _color_map[8] = sf::Color(76, 0, 153);
    }
    sf::Color get_rand_color()
    {
        return this->_color_map[rand() % 9];
    }
    ~ColorGenerator(){}
};

class Plot
{
private:
    Queue<Token*> _infix;

public:
    RPN _rpn;
    Plot(){}
    Plot(Queue<Token*> infix);
    sf::VertexArray operator()(float low, float high, float zoom_factor, bool& error);
    ~Plot();
};





#endif