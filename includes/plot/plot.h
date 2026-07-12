#ifndef PLOT_H
#define PLOT_H
#include "../lib/shunting_yard/shunting_yard.h"
#include "../lib/rpn/rpn.h"
#include "../config/config.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdlib>


struct ColorGenerator
{
    std::array<sf::Color, 9> _colors = {
        sf::Color(94, 234, 212),
        sf::Color(248, 250, 252),
        sf::Color(125, 211, 252),
        sf::Color(196, 181, 253),
        sf::Color(253, 186, 116),
        sf::Color(244, 114, 182),
        sf::Color(163, 230, 53),
        sf::Color(251, 191, 36),
        sf::Color(165, 180, 252),
    };
    sf::Color get_rand_color()
    {
        return this->_colors[static_cast<std::size_t>(std::rand()) % this->_colors.size()];
    }
};

class Plot
{
private:
    Queue<Token*> _infix;

public:
    RPN _rpn;
    Plot(){}
    Plot(Queue<Token*> infix);
    static sf::Vector2f map_point(float x, float y, float low, float high,
                                  float zoom_factor);
    static bool is_visible(const sf::Vector2f& point);
    sf::VertexArray operator()(float low, float high, float zoom_factor, bool& error);
    ~Plot();
};





#endif
