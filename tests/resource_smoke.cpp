#include "includes/config/config.h"
#include "includes/input_box/input_box.h"

#include <iostream>
#include <stdexcept>

int main() {
    try {
        Config first;
        Config second;

        sf::Font& first_font = first.get_font(UI_FONT);
        sf::Font& second_font = second.get_font(UI_FONT);
        if (&first_font != &second_font) {
            throw std::runtime_error("Config loaded more than one UI font instance");
        }
        if (first_font.getInfo().family.empty()) {
            throw std::runtime_error("The packaged UI font has no family metadata");
        }

        bool unknown_font_rejected = false;
        try {
            static_cast<void>(first.get_font(999));
        } catch (const std::out_of_range&) {
            unknown_font_rejected = true;
        }
        if (!unknown_font_rejected) {
            throw std::runtime_error("Unknown font resources should be rejected");
        }

        InputBox input(24, {640, 64}, {0, 0}, sf::Color::White,
                       sf::Color::Black, false);
        input.setText("sin(x)");
        if (input.getText() != "sin(x)") {
            throw std::runtime_error(
                "Programmatic input updates must preserve the submitted expression"
            );
        }

        std::cout << "PASS: packaged UI font and synchronized input state verified"
                  << std::endl;
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << std::endl;
        return 1;
    }
}
