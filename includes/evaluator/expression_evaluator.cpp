#include "expression_evaluator.h"

#include "includes/lib/rpn/rpn.h"
#include "includes/lib/shunting_yard/shunting_yard.h"
#include "includes/tokenizer/tokenizer/tokenizer.h"

#include <cmath>
#include <exception>

namespace {

void delete_tokens(Queue<Token*>& tokens) {
    while (!tokens.empty()) {
        delete tokens.pop();
    }
}

} // namespace

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
            delete_tokens(infix);
            return {false, 0.0, "Expression has invalid syntax"};
        }

        RPN evaluator(postfix);
        const double value = evaluator.rpn(x);
        const bool valid = !evaluator.is_error() && std::isfinite(value);
        delete_tokens(infix);

        if (!valid) {
            return {false, 0.0, "Expression could not be evaluated"};
        }

        return {true, value, {}};
    } catch (const std::exception& error) {
        delete_tokens(infix);
        return {false, 0.0, error.what()};
    }
}
