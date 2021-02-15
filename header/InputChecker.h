#ifndef InputChecker_H
#define InputChecker_H

#include <string>

enum class State 
{
    Initial = 0,
    Value,
    Expression
};

class InputChecker
{
public:
    InputChecker() : 
        m_error_index(0) {}
    ~InputChecker() = default;

    InputChecker(const InputChecker &) = delete;
    InputChecker &operator=(const InputChecker &) = delete;

    bool check(const std::string &input);
    int getErrorIndex() const { return m_error_index; }
    std::string getLastError() const { return m_last_error; }

private:
    int m_error_index;
    std::string m_last_error;
    struct Flags {
        bool int_part_start {false};
        bool value_found {false};
        bool zero_int {false};
        bool point_found {false};
        bool sign_found {false};
        bool minus_found {false};
    };

    bool isSign(const char &c);
    bool isDecimalPoint(const char &c);
};

#endif //InputChecker_H