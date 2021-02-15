#include "../header/stdafx.h"
#include "../header/InputChecker.h"

#include <cctype>

bool InputChecker::check(const std::string &input)
{
    State current_state {State::Initial};
    Flags flags;
    int parentheses {0};
    char last_parentheses = '\0';
    for (int i = 0; i < input.size(); ++i, ++m_error_index) {
        if (current_state == State::Initial) {
            if (input[i] == '(') {
                last_parentheses = input[i];
                ++parentheses;
                current_state = State::Expression;
            } else if (input[i] == '-') {
                flags.minus_found = true;
                current_state = State::Value;
            } else if (isdigit(input[i])) {
                if (input[i] != '0') {
                    flags.int_part_start = true;
                } else if (input[i] == '0' && !flags.zero_int && !flags.int_part_start) {
                    flags.zero_int = true;
                }
                current_state = State::Value;
            } else if (isspace(input[i])) {
                current_state = State::Initial;
            } else {
                m_last_error = "Error: incorrect input\n";
                return false;
            }
        } else if (current_state == State::Value) {
            if (isdigit(input[i])) {
                if (input[i] != '0') {
                    flags.int_part_start = true;
                } else if (input[i] == '0' && !flags.zero_int && !flags.int_part_start) {
                    flags.zero_int = true;
                } else if (input[i] == '0' && flags.zero_int && !flags.point_found) {
                    m_last_error = "Error: too much zero in number\n";
                    return false;
                }
                flags.sign_found = false;
                current_state = State::Value;
            } else if (isDecimalPoint(input[i])) {
                if (flags.point_found) {
                    m_last_error = "Error: too much point in number\n";
                    return false;
                } else if (!isdigit(input[i-1])) {
                    m_last_error = "Error: before point must be a number\n";
                    return false;
                }
                flags.point_found = true;
                current_state = State::Value;
            } else if (isspace(input[i])) {
                if (isdigit(input[i - 1])) {
                    flags.value_found = true;
                    flags.int_part_start = false;
                    flags.zero_int = false;
                    flags.point_found = false;
                    flags.sign_found = false;
                    flags.minus_found = false;
                    current_state = State::Expression;
                } else if (isDecimalPoint(input[i - 1])) {
                    m_last_error = "Error: number value can't end with point\n";
                    return false;
                } else if (input[i - 1] == '-' && flags.minus_found) {
                    m_last_error = "Error: after unary minus must be a digit\n";
                    return false;
                }
            } else if (isSign(input[i])) {
                if (isdigit(input[i - 1])) {
                    flags.value_found = true;
                    flags.int_part_start = false;
                    flags.zero_int = false;
                    flags.sign_found = true;
                    flags.point_found = false;
                    flags.minus_found = false;
                    current_state = State::Expression;
                } else if (isDecimalPoint(input[i - 1])) {
                    m_last_error = "Error: number value can't end with point\n";
                    return false;
                } else if (isSign(input[i - 1])) {
                    m_last_error = "Error: too much sign one after another\n";
                    return false;
                }
            } else if (input[i] == '(') {
                if (isdigit(input[i-1]) || isDecimalPoint(input[i-1]) ||
                   (input[i-1] == '-' && flags.minus_found))
                {
                    m_last_error = "Error: '(' cant't be part of value\n";
                    return false;
                }
                last_parentheses = '(';
            } else if (input[i] == ')') {
                if (isDecimalPoint(input[i - 1])) {
                    m_last_error = "Error: value can't end with point\n";
                    return false;
                } else if (input[i - 1] == '-' && flags.minus_found) {
                    m_last_error = "Error: ')' can't be part of value\n";
                    return false;
                } else if (isdigit(input[i - 1])) {
                    if(--parentheses < 0) {
                        m_last_error = "Error: missing '('\n";
                        return false;
                    }
                    flags.value_found = true;
                    flags.int_part_start = false;
                    flags.zero_int = false;
                    flags.sign_found = false;
                    flags.minus_found = false;
                    flags.point_found = false;
                    last_parentheses = ')';
                    current_state = State::Expression;
                }
            } else {
                m_last_error = "Error: input incorrect\n";
                return false;
            }
        } else if (current_state == State::Expression) {
            if (isSign(input[i])) {
                if (flags.sign_found && input[i] != '-') {
                    m_last_error = "Error: between signs must be a value\n";
                    return false;
                } else if (flags.sign_found && input[i] == '-' && isSign(input[i - 1])) {
                    m_last_error = "Error: between unary minus and arithmetic sign must be a parentheses or space\n";
                    return false;
                } else if (flags.sign_found && !flags.minus_found && input[i] == '-') {
                    flags.minus_found = true;
                    current_state = State::Value;
                } else if (!flags.sign_found && last_parentheses == '(' && !flags.minus_found && input[i] == '-' && !flags.value_found) {
                    flags.minus_found = true;
                    current_state = State::Value;
                } else if (!flags.sign_found && last_parentheses == '(' && flags.value_found) {
                    flags.sign_found = true;
                    current_state = State::Expression;
                } else if (!flags.sign_found && last_parentheses == '(') {
                    m_last_error = "Error: after open parentheses must be a value\n";
                    return false;
                } else if (!flags.sign_found && (last_parentheses == ')' || last_parentheses == '\0')) {
                    flags.sign_found = true;
                    current_state = State::Expression;
                } 
            } else if (isDecimalPoint(input[i])) {
                m_last_error = "Error: decimal point must be inside number value\n";
                return false;
            } else if (isdigit(input[i])) {
                if (input[i] != '0') {
                    flags.int_part_start = true;
                } else if (input[i] == '0' && !flags.zero_int && !flags.int_part_start) {
                    flags.zero_int = true;
                }
                if (flags.sign_found || (!flags.sign_found && last_parentheses == '(')) {
                    current_state = State::Value;
                } else if (!flags.sign_found && (last_parentheses == ')' || last_parentheses == '\0')) {
                    m_last_error = "Error: before value must be a sign\n";
                    return false;
                }
            } else if (input[i] == '(') {
                if (!flags.sign_found && (last_parentheses == ')' || last_parentheses == '\0')) {
                    m_last_error = "Error: before '(' must be a sign\n";
                    return false;
                }
                if (flags.sign_found) {
                    flags.sign_found = false;
                }
                flags.value_found = false;
                ++parentheses;
                last_parentheses = input[i];
                current_state == State::Expression;
            } else if (input[i] == ')') {
                if (flags.sign_found) {
                    m_last_error = "Error: before cannot be a ')'\n";
                    return false;
                } else if (!flags.sign_found && last_parentheses == '(') {
                    m_last_error = "Error: between two parentheses must be a value or expression\n";
                    return false;
                }
                if (--parentheses < 0) {
                    m_last_error = "Error: missing '('\n";
                    return false;
                }
                last_parentheses = input[i];
                current_state = State::Expression;
            } else if (isspace(input[i])) {
                current_state = State::Expression;
            } else {
                m_last_error = "Error: incorrect input\n";
                return false;
            }
        }
    }
    if (parentheses != 0) {
        m_last_error = "Error: wrong parentheses consequence\n";
        return false;
    }
    if (input.back() == '.' || input.back() == ',') {
        m_last_error = "Error: after decimal point must be a number\n";
        return false;
    }

    return true;
}

bool InputChecker::isSign(const char &c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool InputChecker::isDecimalPoint(const char &c)
{
    return (c == '.' || c == ','); 
}
