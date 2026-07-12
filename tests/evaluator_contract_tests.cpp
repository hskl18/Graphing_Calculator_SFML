#include "includes/evaluator/expression_evaluator.h"
#include "includes/lib/lru/lru.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr double TOLERANCE = 1e-9;
int failures = 0;

void fail(const std::string& message) {
    ++failures;
    std::cerr << "FAIL: " << message << std::endl;
}

void expect_success(
    const ExpressionEvaluator& evaluator,
    const std::string& expression,
    double expected,
    double x = 0.0
) {
    const EvaluationResult result = evaluator.evaluate(expression, x);
    if (!result.success) {
        fail(expression + " should succeed, but returned: " + result.error);
        return;
    }
    if (!std::isfinite(result.value) || std::abs(result.value - expected) > TOLERANCE) {
        fail(
            expression + " should equal " + std::to_string(expected) +
            ", but returned " + std::to_string(result.value)
        );
    }
}

void expect_rejected(
    const ExpressionEvaluator& evaluator,
    const std::string& expression
) {
    const EvaluationResult result = evaluator.evaluate(expression);
    if (result.success) {
        fail(
            expression + " should be rejected, but returned " +
            std::to_string(result.value)
        );
    }
}

void test_all_documented_functions() {
    const ExpressionEvaluator evaluator;
    constexpr double PI = 3.14159265358979323846;

    expect_success(evaluator, "sin(0)", 0.0);
    expect_success(evaluator, "cos(0)", 1.0);
    expect_success(evaluator, "tan(0)", 0.0);
    expect_success(evaluator, "cot(0.7853981633974483)", 1.0);
    expect_success(evaluator, "sec(0)", 1.0);
    expect_success(evaluator, "csc(1.5707963267948966)", 1.0);
    expect_success(evaluator, "arcsin(1)", PI / 2.0);
    expect_success(evaluator, "arccos(1)", 0.0);
    expect_success(evaluator, "arctan(1)", PI / 4.0);
    expect_success(evaluator, "sinh(0)", 0.0);
    expect_success(evaluator, "cosh(0)", 1.0);
    expect_success(evaluator, "tanh(0)", 0.0);
    expect_success(evaluator, "ln(2.718281828459045)", 1.0);
    expect_success(evaluator, "log(100)", 2.0);
    expect_success(evaluator, "abs(-3.5)", 3.5);
    expect_success(evaluator, "min(2,5)", 2.0);
    expect_success(evaluator, "max(2,5)", 5.0);
}

void test_strict_number_grammar() {
    const ExpressionEvaluator evaluator;

    expect_success(evaluator, "1", 1.0);
    expect_success(evaluator, "1.", 1.0);
    expect_success(evaluator, ".5", 0.5);
    expect_success(evaluator, "1.5", 1.5);
    expect_rejected(evaluator, "1.2.3");
    expect_rejected(evaluator, "2..3");
}

void test_function_parentheses_and_arity() {
    const ExpressionEvaluator evaluator;

    expect_success(evaluator, "sin(0)", 0.0);
    expect_success(evaluator, "max(1,2)", 2.0);

    for (const std::string expression : {
             "sin 0",
             "sin()",
             "sin(0,1)",
             "max(1)",
             "max(1,2,3)",
             "max(1,,2)",
             "max(1 2)",
             "max(,2)",
             "max(1,)",
         }) {
        expect_rejected(evaluator, expression);
    }
}

void test_unary_minus_and_precedence() {
    const ExpressionEvaluator evaluator;

    expect_success(evaluator, "-x", -3.0, 3.0);
    expect_success(evaluator, "-(2+3)", -5.0);
    expect_success(evaluator, "2^-2", 0.25);
    expect_success(evaluator, "-2^2", -4.0);
    expect_success(evaluator, "(-2)^2", 4.0);
}

void test_invalid_adjacency_and_implicit_multiplication() {
    const ExpressionEvaluator evaluator;

    for (const std::string expression : {
             "2x",
             "2(3+4)",
             "(2)(3)",
             "sin(0)cos(0)",
             "x x",
         }) {
        expect_rejected(evaluator, expression);
    }
}

void test_unmatched_delimiters_and_non_finite_domains() {
    const ExpressionEvaluator evaluator;

    for (const std::string expression : {
             "(2+3",
             "2+3)",
             "sin(0",
             "max(1,2",
             "1/0",
             "0/0",
             "ln(0)",
             "ln(-1)",
             "log(0)",
             "arcsin(2)",
             "arccos(-2)",
             "csc(0)",
         }) {
        expect_rejected(evaluator, expression);
    }
}

void test_zero_capacity_lru_stays_empty() {
    LRU<std::string, int> cache(0);
    cache.put("ignored", 1);

    if (!cache.list_to_vec().empty()) {
        fail("a zero-capacity LRU should not retain inserted entries");
    }

    bool missing = false;
    try {
        static_cast<void>(cache.get("ignored"));
    } catch (const std::out_of_range&) {
        missing = true;
    }
    if (!missing) {
        fail("a zero-capacity LRU lookup should report the entry as missing");
    }
}

} // namespace

int main() {
    test_all_documented_functions();
    test_strict_number_grammar();
    test_function_parentheses_and_arity();
    test_unary_minus_and_precedence();
    test_invalid_adjacency_and_implicit_multiplication();
    test_unmatched_delimiters_and_non_finite_domains();
    test_zero_capacity_lru_stays_empty();

    if (failures != 0) {
        std::cerr << "FAIL: evaluator contract had " << failures
                  << " failing assertion(s)" << std::endl;
        return 1;
    }

    std::cout << "PASS: evaluator contract" << std::endl;
    return 0;
}
