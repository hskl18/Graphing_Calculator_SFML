#include "plot.h"


Plot::Plot(Queue<Token*> infix)
{
    this->_infix = infix;
}

Plot::~Plot()
{
    this->_infix.clear();
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

        // if low and high up, zoom factor down
        // if low and high down, zoom factor up
        float zoom_factor_x = (high + 35) / zoom_factor;
        float zoom_factor_y = (high + 35) / zoom_factor; //+ 37.f * zoom_factor

        float Y = -1.f*y*zoom_factor_y + WINDOW_HEIGHT / 2;
        float X = x*zoom_factor_x + WINDOW_WIDTH/2;
        sf::Vertex point(sf::Vector2f(X, Y));


        point.color = color;
        if(X >= 1000) continue;
        if(fabs(Y) >= 900 || fabs(Y) < 100) continue;
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
