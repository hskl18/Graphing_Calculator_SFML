#ifndef ENGINE_H
#define ENGINE_H
#include <SFML/Graphics.hpp>
#include "../config/config.h"
#include "../input_box/input_box.h"
#include "../plot/plot.h"
#include "../history_bar/history_bar.h"
#include "../tokenizer/tokenizer/tokenizer.h"
#include "../lib/lru/lru.h"
using namespace std;


class Engine 
{
private:
    // sfml
    sf::RenderWindow _window;
    sf::Sprite _error_image;
    sf::VertexArray _points;
    sf::RectangleShape _y_axis;
    sf::RectangleShape _x_axis;

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
    void _update_input_box();
    void _update_history_box();
public:
    Engine();
    ~Engine();
    void input();
    void run();
    void display();
};




#endif