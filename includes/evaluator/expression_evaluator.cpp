#include "expression_evaluator.h"

#include "includes/lib/rpn/rpn.h"
#include "includes/lib/shunting_yard/shunting_yard.h"
#include "includes/tokenizer/tokenizer/tokenizer.h"

#include <cmath>
#include <exception>

EvaluationResult ExpressionEvaluator::evaluate(
    const std::string& expression,
    double x
) const {
    Queue<Token*> infix;

    try {
        Tokenizer tokenizer(expression);
        infix = tokenizer.infix();

        if (infix.empty()) {
            return {false, 0.0, "Expression is empty or contains invalid tokens"};
        }

        ShuntingYard shunting_yard(infix);
        Queue<Token*> postfix = shunting_yard.postfix();

        if (shunting_yard.is_error() || postfix.empty()) {
            return {false, 0.0, "Expression has invalid syntax"};
        }

        RPN evaluator(postfix);
        const double value = evaluator.rpn(x);
        const bool valid = !evaluator.is_error() && std::isfinite(value);
        if (!valid) {
            return {false, 0.0, "Expression could not be evaluated"};
        }

        return {true, value, {}};
    } catch (const std::exception& error) {
        return {false, 0.0, error.what()};
    }
}
