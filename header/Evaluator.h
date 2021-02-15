#ifndef Evaluator_H
#define Evaluator_H

#include "Calculator.h"

#include <string>

class Evaluator
{
public:
    explicit Evaluator() {}
    ~Evaluator() = default;

    Evaluator(const Evaluator &) = delete;
    Evaluator &operator=(const Evaluator &) = delete;

    Value evaluate(const std::string &input);
private:
    Calculator m_calculator;
    int m_expr_pos {0};

    Value getExpression(const std::string &input);
    Value getSum(const std::string &input);
    Value getFactor(const std::string &input);
    Value getNumber(const std::string &input);
};

#endif //Evaluator_H