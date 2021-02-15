#include "../header/stdafx.h"
#include "../header/Calculator.h"

#include <memory>
#include <utility>

static constexpr int MAX_FLOAT_PART_LENGTH {2};

void printValue(const Value &value)
{
    if (value.integer_part.empty() && value.float_part.empty()) {
        std::cout << "Value is empty\n";
        return;
    }

    if (value.is_negative) {
        std::cout << "-";
    }
    for (const auto &integer : value.integer_part) {
        std::cout << integer;
    }
    if (!value.float_part.empty()) {
        std::cout << ".";
        for (const auto &float_unit : value.float_part) {
            std::cout << float_unit;
        }
    }
    std::cout << std::endl;
}

bool isNull(const Value &value)
{
    if (value.integer_part.size() == 1 && value.integer_part[0] == 0) {
        return true;
    }

    return false;
}

void swap(Value &val_1, Value &val_2) noexcept
{
    Value tmp {std::move(val_2)};
    val_2 = std::move(val_1);
    val_1 = std::move(tmp);
}

template<typename SrcContainer, typename DstContainer>
static void copyFromSrcToDst(const SrcContainer &src, DstContainer &dst)
{
    dst.reserve(src.size());
    for (const auto &val : src) {
        dst.push_back(val);
    }
}

//remove unnecessart zero from the front of integer part of value
//if value have a float part, then one zero can be in the integer part
static void removeUnnecessaryZeroFromIntPart(ResultValuePart &int_res, const ValuePart &float_part)
{
    while (!int_res.empty() && int_res.front() == 0
            && int_res.size() > 1) {
        int_res.pop_front();
    }
}

template<typename Container>
static void roundOffFloatPart(Container &float_res)
{
    //remove unnecessary zero numbers from the end of float part
     while (!float_res.empty() && float_res.back() == 0) {
        float_res.pop_back();
    }
    //round off float part
    if (float_res.size() > MAX_FLOAT_PART_LENGTH) {
        ValuePartUnit val = 0;
        for (auto back_it = float_res.rbegin(); back_it != float_res.rend(); ++back_it) {
            if (back_it != --float_res.rend() && val > 5) {
                (*back_it)++;
            }
            val = *back_it;
        }   
        while (float_res.size() > MAX_FLOAT_PART_LENGTH) {
            float_res.pop_back();
        }
    }
}

// Value::Value(int int_part, int fl_part, bool is_neg) :
//     is_negative(is_neg)
// {
//     ResultValuePart int_val, float_val;
//     do {
//         int_val.push_front(int_part % 10);
//         int_part /= 10;
//     } while (int_part);
//     copyFromSrcToDst(int_val, integer_part);

//     while (fl_part) {
//         float_val.push_front(fl_part % 10);
//         fl_part /= 10;
//     }
//     copyFromSrcToDst(float_val, float_part);
// }

bool Value::operator==(const Value &val) const
{
    return (integer_part == val.integer_part) && (float_part == val.float_part) && (is_negative == val.is_negative);
}

Value Calculator::sum(Value &val_1, Value &val_2)
{
    Value result;
    if (val_1.is_negative && val_2.is_negative) {
        result.is_negative = true;
    } else if (!val_1.is_negative && val_2.is_negative) {
        val_2.is_negative = false;
        return diff(val_1, val_2);
    } else if (val_1.is_negative && !val_2.is_negative) {
        val_1.is_negative = false;
        return diff(val_2, val_1);
    }
    ResultValuePart int_res, float_res;
    ValuePartUnit rest = 0;
    
    floatPartSum(val_1.float_part, val_2.float_part, float_res, rest);
    roundOffFloatPart(float_res);
    copyFromSrcToDst(float_res, result.float_part);

    integerPartSum(val_1.integer_part, val_2.integer_part, int_res, rest);
    removeUnnecessaryZeroFromIntPart(int_res, result.float_part);
    copyFromSrcToDst(int_res, result.integer_part);


    return result;
}

void Calculator::integerPartSum(const ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res, ValuePartUnit &rest)
{
    int idx_1 = val_part_1.size() - 1;
    int idx_2 = val_part_2.size() - 1;

    while (idx_1 >= 0 || idx_2 >= 0 || rest) {
        int current_sum = getValuePartUnit(val_part_1, idx_1--) + getValuePartUnit(val_part_2, idx_2--) + rest;
        res.push_front(current_sum % 10);
        rest = current_sum / 10;
    }
}

void Calculator::floatPartSum(const ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res, ValuePartUnit &rest)
{
    int max_idx = val_part_1.size() > val_part_2.size() ? val_part_1.size() - 1 : val_part_2.size() - 1;
    while(max_idx >= 0) {
        auto current_sum = getValuePartUnit(val_part_1, max_idx) + getValuePartUnit(val_part_2, max_idx) + rest;
        res.push_front(current_sum % 10);
        rest = current_sum / 10;
        --max_idx;
    }
}

static void fillFloatPart(ValuePart &val_part, const std::size_t max_float_part_length = MAX_FLOAT_PART_LENGTH)
{
    for (int i = val_part.size(); i < max_float_part_length; ++i) {
        val_part.push_back(0);
    }
} 

Value Calculator::diff(Value &val_1, Value &val_2)
{
    Value result;
    if (val_1.is_negative && val_2.is_negative) {
        val_1.is_negative = false;
        val_2.is_negative = false;
        swap(val_1, val_2);
    } else if (val_1.is_negative && !val_2.is_negative) {
        val_1.is_negative = false;
        result = sum(val_1, val_2);
        result.is_negative = true;
        return result;
    } else if (!val_1.is_negative && val_2.is_negative) {
        val_2.is_negative = false;
        result = sum(val_1, val_2);
        return result;
    }
    auto compareResult = compareValues(val_1, val_2);
    ResultValuePart int_res, float_res;

    auto max_float_part_length = std::max(val_1.float_part.size(), val_2.float_part.size());
    if (val_1.float_part.size() != val_2.float_part.size()) {
        if (val_1.float_part.size() < val_2.float_part.size()) {
            fillFloatPart(val_1.float_part, max_float_part_length);
        } else {
            fillFloatPart(val_2.float_part, max_float_part_length);
        }
    }

    if (compareResult == CompareResult::Greater ||
        compareResult == CompareResult::Equally) {
        floatPartDiff(val_1.integer_part, val_1.float_part, val_2.float_part, float_res);
        integerPartDiff(val_1.integer_part, val_2.integer_part, int_res);
    } else {
        result.is_negative = true;
        floatPartDiff(val_2.integer_part, val_2.float_part, val_1.float_part, float_res);
        integerPartDiff(val_2.integer_part, val_1.integer_part, int_res);
    }

    roundOffFloatPart(float_res);
    copyFromSrcToDst(float_res, result.float_part);
    removeUnnecessaryZeroFromIntPart(int_res, result.float_part);
    copyFromSrcToDst(int_res, result.integer_part);

    return result;
}

static bool getValueFromNextRank(ValuePart &val_part, const int idx)
{
    if (val_part[idx - 1] == 0) {
        for(int i = idx - 1; i >= 0; --i){
            if (val_part[i] == 0) {
                val_part[i] = 9;
                //theese need when value get for float part of value
                //and if first number in value is zero, then need reduce
                //value numbers from integer part
                if (i == 0) {
                    return true;
                }
            } else {
                val_part[i]--;
                break;
            }
        }
    } else {
        val_part[idx - 1]--;
    }

    return false;
}

//val_part_2 always difference from val_part_1.
//val_part_1 always Greater or equally val_part_2.
void Calculator::integerPartDiff(ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res)
{
    int idx_1 = val_part_1.size() - 1;
    int idx_2 = val_part_2.size() - 1;

    int rest = 0;
    while (idx_1 >= 0 || idx_2 >= 0) {
        auto current_val_1 = getValuePartUnit(val_part_1, idx_1);
        auto current_val_2 = getValuePartUnit(val_part_2, idx_2);
        if (current_val_1 < current_val_2) {
            getValueFromNextRank(val_part_1, idx_1);
            current_val_1 += 10;
        }
        int current_difference = current_val_1 - current_val_2;
        res.push_front(current_difference);
        --idx_1;
        --idx_2;
    }
}

void Calculator::floatPartDiff(ValuePart &integer_part_1, ValuePart &val_part_1, const ValuePart &val_part_2, ResultValuePart &res)
{
    int idx_1 = val_part_1.size() - 1;
    int idx_2 = val_part_2.size() - 1;

    int rest = 0;
    while (idx_1 >= 0 || idx_2 >= 0) {
        auto current_val_1 = getValuePartUnit(val_part_1, idx_1);
        auto current_val_2 = getValuePartUnit(val_part_2, idx_2);
        if (current_val_1 < current_val_2) {
            if (idx_1 == 0) {
                getValueFromNextRank(integer_part_1, integer_part_1.size());
            } else {
                if (getValueFromNextRank(val_part_1, idx_1)) {
                    getValueFromNextRank(integer_part_1, integer_part_1.size());
                }
            }
            current_val_1 += 10;
        }
        int current_difference = current_val_1 - current_val_2;
        res.push_front(current_difference);
        --idx_1;
        --idx_2;
    }
    //remove unnecessary zero from back
    // while (!res.empty() && res.back() == 0) {
    //     res.pop_back();
    // }
    // //round off float part
    // if (res.size() > MAX_FLOAT_PART_LENGTH) {
    //     ValuePartUnit val = 0;
    //     for (auto back_it = res.rbegin(); back_it != res.rend(); ++back_it) {
    //         if (back_it != --res.rend() && val > 5) {
    //             (*back_it)++;
    //         }
    //         val = *back_it;
    //     }   
    //     while (res.size() > MAX_FLOAT_PART_LENGTH) {
    //         res.pop_back();
    //     }
    // }
}

ValuePartUnit Calculator::getValuePartUnit(const ValuePart &val_part, const int idx)
{
    return (idx >= 0 && idx < val_part.size()) ? val_part[idx] : 0;
}

//Compare val_1 with val_2
Calculator::CompareResult Calculator::compareValues(const Value &val_1, const Value &val_2)
{
    //if integer parts differ between values, then one of them is greater
    if (val_1.integer_part.size() != val_2.integer_part.size()) {
        return val_1.integer_part.size() > val_2.integer_part.size() ? CompareResult::Greater : CompareResult::Less;
    }

    //else we need to compare unit by unit integer parts of compared values
    for (int i = 0; i < val_1.integer_part.size(); ++i) {
        if (val_1.integer_part[i] > val_2.integer_part[i]) {
            return CompareResult::Greater;
        } else if (val_1.integer_part[i] < val_2.integer_part[i]) {
            return CompareResult::Less;
        }
    }

    //if integer parts are equally then compare float parts
    //first, if one of float part empty then another value greater
    if (val_1.float_part.empty() && !val_2.float_part.empty()) {
        return CompareResult::Less;
    } else if (!val_1.float_part.empty() && val_2.float_part.empty()) {
        return CompareResult::Greater;
    }

    int min_size = std::min(val_1.float_part.size(), val_2.float_part.size());

    for (int i = 0; i < min_size; ++i) {
        if (val_1.float_part[i] > val_2.float_part[i]) {
            return CompareResult::Greater;
        } else if (val_1.float_part[i] < val_2.float_part[i]) {
            return CompareResult::Less;
        }
    }

    return CompareResult::Equally;
}

Value Calculator::multiple(Value &val_1, Value &val_2)
{
    Value result;

    std::vector<ValuePartUnit> full_value_1, full_value_2;
    //fix how much number after decimal dot in value
    int floatPartNum = val_1.float_part.size() + val_2.float_part.size();
    //copy integer and float part into generally vector for further work
    full_value_1.reserve(val_1.integer_part.size() + val_1.float_part.size());
    full_value_2.reserve(val_2.integer_part.size() + val_2.float_part.size());
    copyFromSrcToDst(val_1.integer_part, full_value_1);
    copyFromSrcToDst(val_1.float_part, full_value_1);
    copyFromSrcToDst(val_2.integer_part, full_value_2);
    copyFromSrcToDst(val_2.float_part, full_value_2);

    std::vector<ResultValuePart> results(full_value_2.size());
    ValuePartUnit rest = 0;
    //just like when we manually multiply numbers on paper, we multiply the individual numbers sequentially between each other
    //for example:
    // 54
    //*
    // 16
    // --
    //324
    //54
    //--
    //864
    for (int i = full_value_2.size() - 1, r = 0; i >= 0; --i, ++r) {
        rest = 0;
        for (int f = full_value_2.size() - 1; f > i; --f) {
            results[r].push_front(0);
        }
        for (int j = full_value_1.size() - 1; j >= 0; --j) {
            ValuePartUnit current_multiple = (full_value_1[j] * full_value_2[i]) + rest;
            results[r].push_front(current_multiple % 10);
            rest = current_multiple / 10;
        }
        if (rest != 0) {
            results[r].push_front(rest);
        }
    }
    
    //copy temp results to the "Value" vector for further sum result values, that we take on previous step
    std::vector<Value> tmpValues(results.size());
    for (int i = 0; i < results.size(); ++i) {
        copyFromSrcToDst(results[i], tmpValues[i].integer_part);
    }

    Value tmp;
    for (int i = 0; i < tmpValues.size();) {
        if (i == 0) {
            //if multiple values with only one number then don't need sum
            tmp = (tmpValues.size() == 1) ? tmpValues[i] : sum(tmpValues[i], tmpValues[i + 1]);
            i += 2;
        } else {
            tmp = sum(tmp, tmpValues[i]);
            ++i;
        }
    }

    for (int i = tmp.integer_part.size() - floatPartNum, c = 0; i < tmp.integer_part.size() && c < MAX_FLOAT_PART_LENGTH; ++i, ++c) {
        result.float_part.push_back(tmp.integer_part[i]);
    }
    roundOffFloatPart(result.float_part);
    for (int i = 0; i < tmp.integer_part.size() - floatPartNum; ++i) {
        result.integer_part.push_back(tmp.integer_part[i]);
    }

    //round off the float part, if necessary
    // if ((tmp.integer_part.size() - floatPartNum) > MAX_FLOAT_PART_LENGTH) {
    //     for (int i = tmp.integer_part.size() - 1; i > tmp.integer_part.size()  - floatPartNum + 1; --i) {
    //         if (tmp.integer_part[i] > 5) {
    //             tmp.integer_part[i - 1]++;
    //         }
    //     }
    // }

    // while (!result.float_part.empty() && result.float_part.back() == 0) {
    //     result.float_part.pop_back();
    // } 
    if ((val_1.is_negative && val_2.is_negative) ||
        (!val_1.is_negative && !val_2.is_negative)) {
        result.is_negative = false;
    } else if ((!val_1.is_negative && val_2.is_negative) ||
               (val_1.is_negative && !val_2.is_negative)) {
        result.is_negative = true;
    }

    return result;
}

Value Calculator::divide(Value &val_1, Value &val_2)
{
    Value result;

    int max_float_part = std::max(val_1.float_part.size(), val_2.float_part.size());
    
    if (max_float_part != 0) {
        if (val_1.integer_part.size() == 1 && val_1.integer_part[0] == 0) {
            val_1.integer_part.clear();
        }
        if (val_2.integer_part.size() == 1 && val_2.integer_part[0] == 0) {
            val_2.integer_part.clear();
        }

        copyFromSrcToDst(val_1.float_part, val_1.integer_part);
        copyFromSrcToDst(val_2.float_part, val_2.integer_part);
        if (val_1.float_part.size() > val_2.float_part.size()) {
            for (int i = 0; i < val_1.float_part.size() - val_2.float_part.size(); ++i) {
                val_2.integer_part.push_back(0);
            }
        } else if (val_1.float_part.size() < val_2.float_part.size()) {
            for (int i = 0; i < val_2.float_part.size() - val_1.float_part.size(); ++i) {
                val_1.integer_part.push_back(0);
            }
        }

        val_1.float_part.clear();
        val_2.float_part.clear();
    }

    auto convertVectorToInt = [] (const std::vector<ValuePartUnit> &vec, std::size_t &val) {
        for (const auto &unit : vec) {
            val = (val * 10) + unit;
        }
    };
    auto convertIntToVector = [] (std::vector<ValuePartUnit> &vec, const std::size_t &val) {
        auto val_copy = val;
        ResultValuePart tmp;
        while (val_copy) {
            tmp.push_front(val_copy % 10);
            val_copy /= 10;
        }
        copyFromSrcToDst(tmp, vec);
    };
    //делитель
    std::size_t divider {0};
    convertVectorToInt(val_2.integer_part, divider);

    //делимое
    std::size_t dividend {0};
    int float_units {0};
    bool float_start {false};
    //start position index on each divide step in source value, which we can use to add numbers to current dividend
    int src_idx_start {0};
    //fact position index on each divide step, which increase after we using number from source value
    int used_src_idx {src_idx_start}; 
    const std::vector<ValuePartUnit> &src_value = val_1.integer_part;
    auto current_value = std::make_shared<Value>();
    copyFromSrcToDst(val_1.integer_part, current_value->integer_part);
    while (float_units < MAX_FLOAT_PART_LENGTH) {
        convertVectorToInt(current_value->integer_part, dividend);
        std::list<ValuePartUnit> rest;
        while ((dividend/divider) > 10) {
            rest.push_front(dividend % 10);
            dividend /= 10;
        }
        bool first_zero_skip = true;
        used_src_idx = src_idx_start;
        while (1) {
            auto factor = dividend / divider;
            if (factor >= 1) {
                auto tmp_2 = factor * divider;
                Value tmp_val_1, tmp_val_2;
                convertIntToVector(tmp_val_1.integer_part, dividend);
                convertIntToVector(tmp_val_2.integer_part, tmp_2);
                current_value = std::make_shared<Value>(diff(tmp_val_1, tmp_val_2));
                std::vector<ValuePartUnit> factor_vec;
                convertIntToVector(factor_vec, factor);
                if (float_start) {
                    float_units++;
                    copyFromSrcToDst(factor_vec, result.float_part);
                    // result.float_part.push_back(factor);
                } else {
                    copyFromSrcToDst(factor_vec, result.integer_part);
                    // result.integer_part.push_back(factor);
                }
                if (isNull(*current_value) && !rest.empty()) {
                    copyFromSrcToDst(rest, current_value->integer_part);
                }
                //offset of the start index for the next step
                //added_pos need to balance of added value. if size of the current dividend is 1, then add 1, else reduce the dividend size by 1
                auto added_pos = tmp_val_1.integer_part.size() == 1 ? 1 : tmp_val_1.integer_part.size() - 1;
                //after first divide step increase src_idx_start by full dividend size, else add to current src_idx_start added_pos offset
                src_idx_start = src_idx_start ? (src_idx_start + added_pos) : tmp_val_1.integer_part.size();
                break;
            } else if (dividend > 0) {
                if (used_src_idx > 0 && used_src_idx < src_value.size()) {
                    dividend = (dividend * 10) + src_value[used_src_idx++];
                } else if (dividend > 0) {
                    dividend *= 10;
                    if (result.integer_part.empty()) {
                        result.integer_part.push_back(0);
                        first_zero_skip = false;
                        float_start = true;
                    } else {
                        if (!first_zero_skip) {
                            float_units++;
                            result.float_part.push_back(0);
                        }
                        first_zero_skip = false;
                        float_start = true;
                    }
                }
            } else {
                //if dividend is 0 then break
                break;
            }
        }
        dividend = 0;
        if (isNull(*current_value) || current_value->integer_part == src_value) {
            if (current_value->integer_part == src_value) {
                result.integer_part.push_back(0);
            }
            break;
        }
    }

    roundOffFloatPart(result.float_part);
    // if (result.float_part.size() > MAX_FLOAT_PART_LENGTH) {
    //     ValuePartUnit val = 0;
    //     for (auto back_it = result.float_part.rbegin(); back_it != result.float_part.rend(); ++back_it) {
    //         if (back_it != (result.float_part.rend() - 1) && val > 5) {
    //             (*back_it)++;
    //         }
    //         val = *back_it;
    //     }
    //     while (result.float_part.size() > MAX_FLOAT_PART_LENGTH) {
    //         result.float_part.pop_back();
    //     }
    // }

    if ((val_1.is_negative && val_2.is_negative) ||
        (!val_1.is_negative && !val_2.is_negative)) {
        result.is_negative = false;
    } else if ((!val_1.is_negative && val_2.is_negative) ||
               (val_1.is_negative && !val_2.is_negative)) {
        result.is_negative = true;
    }

    return result;
}