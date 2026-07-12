#include "engine.h"
#include <stdexcept>


Engine::Engine()
{
    this->_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Graphing Calculator");
    this->_window.setKeyRepeatEnabled(true);

    this->_x_axis.setSize({1000, 2});
    this->_y_axis.setSize({2, 1000});
    this->_x_axis.setPosition(X_AXIS_POS);
    this->_y_axis.setPosition(Y_AXIS_POS);
    this->_x_axis.setFillColor(sf::Color(255,204,203));
    this->_y_axis.setFillColor(sf::Color(255,204,203));
    this->_current_function = "";
    this->_history_bar = HistoryBar();
    this->_config = Config();
    this->_input_box = InputBox(INPUT_BOX_FONT_SIZE, INPUT_BOX_SIZE, INPUT_BOX_POS,
                                sf::Color(241, 245, 249), sf::Color(22, 32, 49), false);

    sf::VertexArray temp(sf::Points, 0);
    temp.clear();
    this->_lru = LRU<string, sf::VertexArray>(5);

    for(int i = 0; i < 5; ++i)
    {
        string unset = "Unset " + to_string(i);
        this->_lru.put(unset, temp);
    }

    this->_error = false;
    this->_need_reset = false;
    this->_zoom_factor = ZOOM_MIN;
    this->_low_bound = -35;
    this->_up_bound = 35;

    this->_error_panel.setSize(ERROR_PANEL_SIZE);
    this->_error_panel.setPosition(ERROR_PANEL_POS);
    this->_error_panel.setFillColor(sf::Color(45, 18, 24));
    this->_error_panel.setOutlineColor(sf::Color(239, 68, 68));
    this->_error_panel.setOutlineThickness(3.0f);

    this->_error_icon.setRadius(42.0f);
    this->_error_icon.setPosition(ERROR_PANEL_POS + sf::Vector2f(118, 30));
    this->_error_icon.setFillColor(sf::Color(220, 38, 38));

    this->_error_mark.setSize({8.0f, 42.0f});
    this->_error_mark.setPosition(ERROR_PANEL_POS + sf::Vector2f(156, 48));
    this->_error_mark.setFillColor(sf::Color::White);

    this->_error_dot.setRadius(5.0f);
    this->_error_dot.setPosition(ERROR_PANEL_POS + sf::Vector2f(155, 100));
    this->_error_dot.setFillColor(sf::Color::White);

    this->_input_box.setFont(this->_config.get_font(UI_FONT));
    this->_input_box.setText("sin(x)");

    this->_x_axis.setFillColor(sf::Color(100, 116, 139));
    this->_y_axis.setFillColor(sf::Color(100, 116, 139));
    for(int x = 100; x < 1000; x += 100)
    {
        sf::RectangleShape line({1.0f, 1000.0f});
        line.setPosition(static_cast<float>(x), 0.0f);
        line.setFillColor(sf::Color(42, 56, 76));
        this->_grid_lines.push_back(line);
    }
    for(int y = 100; y < 1000; y += 100)
    {
        sf::RectangleShape line({1000.0f, 1.0f});
        line.setPosition(0.0f, static_cast<float>(y));
        line.setFillColor(sf::Color(42, 56, 76));
        this->_grid_lines.push_back(line);
    }

    this->_configure_text(this->_product_title, "GRAPHING CALCULATOR", 20, {24, 22},
                          sf::Color(226, 232, 240));
    this->_configure_text(this->_input_label, "FUNCTION", 13, {180, 882},
                          sf::Color(94, 234, 212));
    this->_configure_text(this->_control_hint,
                          "ENTER edit   UP graph   LEFT / RIGHT zoom   DOWN clear", 14,
                          {180, 974}, sf::Color(148, 163, 184));
    this->_configure_text(this->_error_title, "Invalid expression", 20, {414, 478},
                          sf::Color(254, 226, 226));
    this->_configure_text(this->_error_hint, "Check the function syntax and try again.", 13,
                          {367, 510}, sf::Color(254, 202, 202));
}

Engine::~Engine(){}

void Engine::_configure_text(sf::Text& text, const std::string& value, unsigned int size,
                             const sf::Vector2f& position, const sf::Color& color)
{
    text.setFont(this->_config.get_font(UI_FONT));
    text.setString(value);
    text.setCharacterSize(size);
    text.setPosition(position);
    text.setFillColor(color);
}

void Engine::input()
{
    sf::Event event;
    while (this->_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            cout << "User Quit" << endl;
            this->_window.close();
        }
        if (event.type == sf::Event::TextEntered)
        {
            this->_input_box.typedOn(event);
        }
        if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Return:
                    this->_input_box.setSelected(true);
                    break;
                case sf::Keyboard::Escape:
                    this->_input_box.setSelected(false);
                    break;
                case sf::Keyboard::Up:
                    if(!this->_need_reset)
                    {
                        this->_update_equation();
                        if(!this->_error)
                        {
                            this->_lru.put(this->_current_function, this->_points);
                        }
                    }
                    break;
                case sf::Keyboard::Right:
                    if(this->_zoom_factor > ZOOM_MIN)
                    {
                        this->_zoom_factor -= ZOOM_DELTA;
                        this->_zoom_helper();
                    }
                    break;
                case sf::Keyboard::Left:
                    if(this->_zoom_factor < ZOOM_MAX)
                    {
                        this->_zoom_factor += ZOOM_DELTA;
                        this->_zoom_helper();
                    }
                    break;
                case sf::Keyboard::Down:
                    this->_points.clear();
                    this->_need_reset = false;
                    this->_error = false;
                    break;
                default:
                    break;
            }
        }

        int index = -1;
        if(this->_history_bar.isClicked(event, this->_window, index) && index != -1) this->_update_cache(index);
    }
    this->_history_bar.update_buttons(this->_lru.list_to_vec());
}


void Engine::_zoom_helper()
{
    if(!this->_error && this->_points.getVertexCount() != 0) 
    {
        string key = this->_lru.get_key(4);
        this->_current_function = key;
        this->_input_box.setText(this->_current_function);
        this->_points.clear();
        Tokenizer tk(this->_current_function);
        Queue<Token*>infix = tk.infix();


        if(infix.empty()) 
        {
            cout << "Tokenize error" << endl;
            this->_error = true;
            this->_need_reset = true;
            return;
        }

        Plot plot(infix);
        sf::VertexArray points = plot(this->_low_bound, this->_up_bound, this->_zoom_factor, this->_error);
        this->_need_reset = this->_error;
        this->_points = points;
        this->_lru.put(key, this->_points);
        this->_input_box.drawTo(this->_window);
    }
}


void Engine::_update_cache(int index)
{
    string key = this->_lru.get_key(index);
    if(key.substr(0, 5) == "Unset") return;
    this->_current_function = key;
    this->_points = this->_lru.get(key);
    this->_error = false;
    this->_need_reset = false;
    this->_history_bar.update_buttons(this->_lru.list_to_vec());
    //cout << this->_lru << endl;
    this->_input_box.setText(this->_current_function);
}



void Engine::_update_equation()
{
    string func = "";
    this->_points.clear();
    this->_current_function = "";
    func += this->_input_box.getText();
    Tokenizer tk(func);
    Queue<Token*>infix = tk.infix();
    //cout << "infix: " << infix << endl;


    if(infix.empty()) 
    {
        cout << "Tokenize error" << endl;
        this->_error = true;
        this->_need_reset = true;
        return;
    }

    Plot plot(infix);
    sf::VertexArray points = plot(this->_low_bound, this->_up_bound, this->_zoom_factor, this->_error);
    this->_need_reset = this->_error;
    this->_points = points;
    if(!this->_error) this->_current_function = func;
    //cout << endl << "Current function is: " << this->_current_function << endl;
}





void Engine::_draw(sf::RenderTarget& target)
{
    for(const sf::RectangleShape& line : this->_grid_lines)
    {
        target.draw(line);
    }
    target.draw(this->_product_title);
    this->_history_bar.drawTo(target);

    if(!this->_error) target.draw(this->_points);
    target.draw(this->_x_axis);
    target.draw(this->_y_axis);
    if(this->_error)
    {
        target.draw(this->_error_panel);
        target.draw(this->_error_icon);
        target.draw(this->_error_mark);
        target.draw(this->_error_dot);
        target.draw(this->_error_title);
        target.draw(this->_error_hint);
    }
    target.draw(this->_input_label);
    target.draw(this->_control_hint);
    this->_input_box.drawTo(target);
}

void Engine::display()
{
    this->_history_bar.isOver(this->_window);
    this->_draw(this->_window);
}

void Engine::run()
{
    this->_input_box.setLimit(true, CHAR_LIMIT);

    while (this->_window.isOpen())
    {

        this->input();
        this->_window.clear(sf::Color(13, 21, 34));
        this->display();
        this->_window.display();
    }

}

void Engine::smoke_test()
{
    this->_update_equation();
    if(this->_error || this->_points.getVertexCount() == 0)
    {
        throw std::runtime_error("The startup expression could not be graphed");
    }
    this->_lru.put(this->_current_function, this->_points);
    this->_input_box.setText("1.2.3");
    this->_update_equation();
    if(!this->_error)
    {
        throw std::runtime_error("The invalid-expression state was not reached");
    }
    this->_update_cache(4);
    if(this->_error || this->_need_reset || this->_points.getVertexCount() == 0 ||
       this->_input_box.getText() != "sin(x)")
    {
        throw std::runtime_error("A history selection did not restore a valid graph");
    }
    this->_window.clear(sf::Color(13, 21, 34));
    this->display();
    this->_window.display();
    this->_window.close();
}
