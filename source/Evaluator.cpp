#include "../header/stdafx.h"
#include "../header/Evaluator.h"

Value Evaluator::evaluate(const std::string &input)
{
    return getExpression(input);
}

Value Evaluator::getExpression(const std::string &input)
{
    Value res = getSum(input);
    while (input[m_expr_pos] == '+' || input[m_expr_pos] == '-'
           || input[m_expr_pos] == ' ') {
        if (input[m_expr_pos] == '+') {
            ++m_expr_pos;
            auto tmp = getSum(input);
            res = m_calculator.sum(res, tmp);
        } else if (input[m_expr_pos] == '-') {
            ++m_expr_pos;
            auto tmp = getSum(input);
            res = m_calculator.diff(res, tmp);
        } else if (input[m_expr_pos] == ' ' ) {
            ++m_expr_pos;
        }
    }

    return res;
}

Value Evaluator::getSum(const std::string &input)
{
    Value res = getFactor(input);
    while (input[m_expr_pos] == '*' || input[m_expr_pos] == '/'
           || input[m_expr_pos] == ' ') {
        if (input[m_expr_pos] == '*') {
            ++m_expr_pos;
            auto tmp = getFactor(input);
            res = m_calculator.multiple(res, tmp);
        } else if (input[m_expr_pos] == '/') {
            ++m_expr_pos;
            auto tmp = getFactor(input);
            if (isNull(tmp)) {
                throw std::runtime_error("zero divider");
            }
            res = m_calculator.divide(res, tmp);
        } else if (input[m_expr_pos] == ' ' ) {
            ++m_expr_pos;
        }
    }

    return res;
}

Value Evaluator::getFactor(const std::string &input)
{
    while (input[m_expr_pos] == ' ') {
        ++m_expr_pos;
    }
    if (input[m_expr_pos] == '(') {
        ++m_expr_pos;
        auto res = getExpression(input);
        ++m_expr_pos;
        return res;
    } else {
        return getNumber(input);
    }
}

Value Evaluator::getNumber(const std::string &input)
{
    Value res;
    bool float_start = false, value_start = false;
    while (m_expr_pos < input.length() && (isdigit(input[m_expr_pos])
           || input[m_expr_pos] == '-' || input[m_expr_pos] == ','
           || input[m_expr_pos] == '.')) {
        if (input[m_expr_pos] == '-' && value_start) {
            break;
        }
        if (isdigit(input[m_expr_pos])) {
            if (!float_start) {
                res.integer_part.push_back(input[m_expr_pos] - '0');
            } else {
                res.float_part.push_back(input[m_expr_pos] - '0');
            }
            value_start = true;
        } else if (input[m_expr_pos] == '-' && !value_start) {
            res.is_negative = true;
        } else if (input[m_expr_pos] == '.' || input[m_expr_pos] == ',') {
            float_start = true;
        } 
        ++m_expr_pos;
    }

    return res;
}
