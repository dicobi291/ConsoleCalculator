#ifndef Calculator_H
#define Calculator_H

#include <vector>
#include <list>
#include <cstdint>

using ValuePartUnit = uint16_t;
using ValuePart = std::vector<ValuePartUnit>;
using ResultValuePart = std::list<ValuePartUnit>;

struct Value
{
    ValuePart integer_part;
    ValuePart float_part;
    bool is_negative {false};

    bool operator==(const Value &val) const;
};

void swap(Value &val_1, Value &val_2) noexcept;


void printValue(const Value &value);
bool isNull(const Value &value);

class Calculator
{
public:
    Calculator() = default;
    ~Calculator() = default;

    Calculator (const Calculator &) = delete;
    Calculator &operator=(const Calculator &) = delete;

    Value sum(Value &val_1, Value &val_2);
    Value diff(Value &val_1, Value &val_2);
    Value multiple(Value &val_1, Value &val_2);
    Value divide(Value &val_1, Value &val_2);

private:
    enum class CompareResult {Greater = 0, Less, Equally};
    
    void integerPartSum(const ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res, ValuePartUnit &rest);
    void floatPartSum(const ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res, ValuePartUnit &rest);
    
    void integerPartDiff(ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res);
    void floatPartDiff(ValuePart &integer_part_1, ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res);

    CompareResult compareValues(const Value &val_1, const Value &val_2);
    ValuePartUnit getValuePartUnit(const ValuePart &val_part, const int idx);
};

#endif //Calculator_H