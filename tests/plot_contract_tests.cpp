#include "includes/plot/plot.h"
#include "includes/tokenizer/tokenizer/tokenizer.h"

#include <cmath>
#include <iostream>
#include <string>

namespace {

int failures = 0;

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void verify_coordinate_contract() {
    const sf::Vector2f left = Plot::map_point(-35.0f, 0.0f, -35.0f, 35.0f, 1.0f);
    const sf::Vector2f center = Plot::map_point(0.0f, 0.0f, -35.0f, 35.0f, 1.0f);
    const sf::Vector2f right = Plot::map_point(35.0f, 0.0f, -35.0f, 35.0f, 1.0f);

    expect(std::fabs(left.x) < 0.001f, "low x bound maps to the left edge");
    expect(std::fabs(center.x - 500.0f) < 0.001f, "x=0 maps to canvas center");
    expect(std::fabs(right.x - 1000.0f) < 0.001f, "high x bound maps to right edge");
    expect(Plot::is_visible({0.0f, 500.0f}), "left-edge point is visible");
    expect(!Plot::is_visible({-0.1f, 500.0f}), "negative screen x is clipped");
    expect(!Plot::is_visible({500.0f, 900.0f}), "input-region y is clipped");
}

void verify_non_finite_rejection() {
    Tokenizer tokenizer("1/0");
    Plot plot(tokenizer.infix());
    bool error = false;
    const sf::VertexArray points = plot(-35.0f, 35.0f, 1.0f, error);
    expect(error, "non-finite samples report a plotting error");
    expect(points.getVertexCount() == 0, "non-finite samples produce no vertices");
}

void verify_visible_graph_contract() {
    Tokenizer tokenizer("sin(x)");
    Plot plot(tokenizer.infix());
    bool error = false;
    const sf::VertexArray points = plot(-35.0f, 35.0f, 1.0f, error);
    expect(!error, "sin(x) plots without error");
    expect(points.getVertexCount() > 0, "sin(x) produces visible points");
    for (std::size_t i = 0; i < points.getVertexCount(); ++i) {
        const sf::Vector2f position = points[i].position;
        expect(std::isfinite(position.x) && std::isfinite(position.y),
               "plot vertices are finite");
        expect(Plot::is_visible(position), "plot vertices stay inside the canvas");
    }
}

} // namespace

int main() {
    verify_coordinate_contract();
    verify_non_finite_rejection();
    verify_visible_graph_contract();
    if (failures != 0) {
        std::cerr << failures << " plot contract assertion(s) failed\n";
        return 1;
    }
    std::cout << "PASS: plotting coordinates and finite vertices verified\n";
    return 0;
}
