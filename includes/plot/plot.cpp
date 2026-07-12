#include "plot.h"

#include <cmath>


Plot::Plot(Queue<Token*> infix)
{
    this->_infix = infix;
}

Plot::~Plot()
{
    this->_infix.clear();
}

sf::Vector2f Plot::map_point(float x, float y, float low, float high,
                             float zoom_factor)
{
    const float math_center = (low + high) / 2.0f;
    const float scale = 1000.0f / (high - low) / zoom_factor;
    return {
        500.0f + (x - math_center) * scale,
        500.0f - y * scale
    };
}

bool Plot::is_visible(const sf::Vector2f& point)
{
    return std::isfinite(point.x) && std::isfinite(point.y) &&
           point.x >= 0.0f && point.x < 1000.0f &&
           point.y >= 80.0f && point.y < 880.0f;
}

sf::VertexArray Plot::operator()(float low, float high, float zoom_factor, bool& error)
{
    sf::VertexArray function(sf::Points, 80500);
    function.clear();
    ShuntingYard sy(this->_infix);
    Queue<Token*> postfix = sy.postfix();
    if(sy.is_error())
    {
        cout << "error in shunting yard!";
        error = true;
        return function;
    }
    this->_rpn = RPN(postfix);

    ColorGenerator cg;
    sf::Color color = cg.get_rand_color();
    
    for(float x = low; x < high; x+=0.0005)
    {   

        float y = this->_rpn.rpn(x);
        if(this->_rpn.is_error())
        {
            cout << "Error in rpn! " << endl;
            error = true;
            function.clear();
            return function;
        }

        if(!std::isfinite(y)) continue;
        sf::Vertex point(map_point(x, y, low, high, zoom_factor));
        point.color = color;
        if(!is_visible(point.position)) continue;
        function.append(point);
    }

    if(function.getVertexCount() == 0)
    {
        error = true;
        function.clear();
        return function;
    } 
    return function;
}
