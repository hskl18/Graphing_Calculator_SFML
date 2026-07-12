#ifndef ENGINE_H
#define ENGINE_H
#include <SFML/Graphics.hpp>
#include "../config/config.h"
#include "../input_box/input_box.h"
#include "../plot/plot.h"
#include "../history_bar/history_bar.h"
#include "../tokenizer/tokenizer/tokenizer.h"
#include "../lib/lru/lru.h"
#include <vector>
using namespace std;


class Engine 
{
private:
    // sfml
    sf::RenderWindow _window;
    sf::RectangleShape _error_panel;
    sf::CircleShape _error_icon;
    sf::RectangleShape _error_mark;
    sf::CircleShape _error_dot;
    sf::VertexArray _points;
    sf::RectangleShape _y_axis;
    sf::RectangleShape _x_axis;
    std::vector<sf::RectangleShape> _grid_lines;
    sf::Text _product_title;
    sf::Text _input_label;
    sf::Text _control_hint;
    sf::Text _error_title;
    sf::Text _error_hint;

    Config _config;
    InputBox _input_box;
    HistoryBar _history_bar;
    LRU<string, sf::VertexArray> _lru;

    string _current_function;
    bool _error;
    bool _need_reset;
    float _zoom_factor;
    float _low_bound;
    float _up_bound;

    void _zoom_helper();
    void _update_cache(int index);
    void _update_equation();
    void _update_history_box();
    void _configure_text(sf::Text& text, const std::string& value, unsigned int size,
                         const sf::Vector2f& position, const sf::Color& color);
    void _draw(sf::RenderTarget& target);
public:
    Engine();
    ~Engine();
    void input();
    void run();
    void display();
    void smoke_test();
};




#endif
