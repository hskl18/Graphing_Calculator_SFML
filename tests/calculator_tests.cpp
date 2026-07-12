#include "includes/evaluator/expression_evaluator.h"
#include "includes/lib/lru/lru.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void expect_near(double actual, double expected, const std::string& message) {
    if (std::abs(actual - expected) > 1e-9) {
        throw std::runtime_error(message);
    }
}

void expect_true(bool value, const std::string& message) {
    if (!value) {
        throw std::runtime_error(message);
    }
}

void test_expression_precedence_and_variables() {
    ExpressionEvaluator evaluator;

    const EvaluationResult precedence = evaluator.evaluate("2 + 3 * 4");
    expect_true(precedence.success, "precedence expression should succeed");
    expect_near(precedence.value, 14.0, "multiplication should bind first");

    const EvaluationResult grouped = evaluator.evaluate("(2 + 3) * 4");
    expect_true(grouped.success, "grouped expression should succeed");
    expect_near(grouped.value, 20.0, "parentheses should override precedence");

    const EvaluationResult variable = evaluator.evaluate("x^2 + 2*x + 1", 3.0);
    expect_true(variable.success, "variable expression should succeed");
    expect_near(variable.value, 16.0, "variable substitution should be deterministic");
}

void test_lru_order_and_bounds() {
    LRU<std::string, int> cache(2);
    cache.put("first", 1);
    cache.put("second", 2);
    expect_true(cache.get("first") == 1, "cache should return stored values");
    cache.put("third", 3);

    expect_true(cache.get_key(0) == "first", "recent access should protect an entry");
    expect_true(cache.get_key(1) == "third", "new entry should be most recent");

    bool threw = false;
    try {
        static_cast<void>(cache.get_key(2));
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect_true(threw, "out-of-range cache access should throw");
}

void test_functions_unary_values_and_invalid_input() {
    ExpressionEvaluator evaluator;

    const EvaluationResult function = evaluator.evaluate("sin(0) + abs(-3.5)");
    expect_true(function.success, "supported functions should succeed");
    expect_near(function.value, 3.5, "function and unary evaluation should compose");

    const EvaluationResult invalid = evaluator.evaluate("2 +");
    expect_true(!invalid.success, "incomplete expressions should fail");
    expect_true(!invalid.error.empty(), "failed expressions should explain the error");

    const EvaluationResult unmatched = evaluator.evaluate("(2 + 3");
    expect_true(!unmatched.success, "unmatched parentheses should fail");

    const EvaluationResult division_by_zero = evaluator.evaluate("1 / 0");
    expect_true(!division_by_zero.success, "non-finite results should fail");
}

} // namespace

int main() {
    try {
        test_expression_precedence_and_variables();
        std::cout << "PASS: expression precedence and variables" << std::endl;
        test_lru_order_and_bounds();
        std::cout << "PASS: LRU order and bounds" << std::endl;
        test_functions_unary_values_and_invalid_input();
        std::cout << "PASS: functions and invalid input" << std::endl;
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << std::endl;
        return 1;
    }
}
