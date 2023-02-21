#include "engine.h"


Engine::Engine()
{
    this->_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Graphing Calculator");
    this->_window.setKeyRepeatEnabled(true);

    this->_x_axis.setSize({1000, 5});
    this->_y_axis.setSize({5, 1000});
    this->_x_axis.setPosition(X_AXIS_POS);
    this->_y_axis.setPosition(Y_AXIS_POS);
    this->_x_axis.setFillColor(sf::Color::Red);
    this->_y_axis.setFillColor(sf::Color::Red);
    this->_current_function = "";
    this->_history_bar = HistoryBar();
    this->_config = Config();
    this->_input_box = InputBox(INPUT_BOX_FONT_SIZE, INPUT_BOX_SIZE, INPUT_BOX_POS, sf::Color::Red, sf::Color::White, false);

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
    this->_error_image = sf::Sprite(this->_config.get_texture(ERROR_IMAGE));
    this->_error_image.setPosition(ERROR_IMAGE_POS);
}

Engine::~Engine(){}

void Engine::_update_input_box()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        //cout << "User selecting input box" << endl;
        this-> _input_box.setSelected(true);
        return;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        //cout << "User quit inputing" << endl;
        this->_input_box.setSelected(false);
        return;
    }
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
        if(!this->_need_reset && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            cout << "User graphing" << endl;
            this->_update_equation();
            if(!this->_error) this->_lru.put(this->_current_function, this->_points);
            //cout << this->_lru << endl;
        }


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->_zoom_factor > ZOOM_MIN)
        {
            cout << "Zooming in" << endl;
            this->_zoom_factor -= ZOOM_DELTA;
            this->_zoom_helper();
            //cout << this->_lru << endl;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->_zoom_factor < ZOOM_MAX)
        {
            cout << "Zooming out" << endl;
            this->_zoom_factor += ZOOM_DELTA;
            this->_zoom_helper();
            //cout << this->_lru << endl;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            cout << "User reset error state" << endl;
            this->_points.clear();
            this->_need_reset = false;
            this->_error = false;
        }

        this->_history_bar.update_buttons(this->_lru.list_to_vec());

        int index = -1;
        if(this->_history_bar.isClicked(event, this->_window, index) && index != -1) this->_update_cache(index);

        
    }
}


void Engine::_zoom_helper()
{
    if(!this->_error && this->_points.getVertexCount() != 0) 
    {
        string key = this->_lru.get_key(4);
        this->_current_function = key;
        this->_input_box.get_text_box().setString(this->_current_function);
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
    this->_history_bar.update_buttons(this->_lru.list_to_vec());
    //cout << this->_lru << endl;
    this->_input_box.get_text_box().setString(this->_current_function);
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








void Engine::display()
{
    this->_history_bar.drawTo(this->_window);
    this->_input_box.drawTo(this->_window);  
    this->_window.draw(this->_x_axis);
    this->_window.draw(this->_y_axis);
    this->_history_bar.isOver(this->_window);
    
    if(!this->_error) this->_window.draw(this->_points);
    if(this->_error) this->_window.draw(this->_error_image);
    this->_input_box.drawTo(this->_window);
}

void Engine::run()
{
    sf::Font arial = this->_config.get_font(ARIAL);
    this->_input_box.setFont(arial);
    this->_input_box.setLimit(true, CHAR_LIMIT);

    while (this->_window.isOpen())
    {

        this->_update_input_box();
        this->input();
        this->_window.clear();
        this->display();
        this->_window.display();
    }

}