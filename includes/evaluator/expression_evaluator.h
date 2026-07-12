#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include <string>

struct EvaluationResult {
    bool success;
    double value;
    std::string error;
};

class ExpressionEvaluator {
public:
    EvaluationResult evaluate(const std::string& expression, double x = 0.0) const;
};

#endif
