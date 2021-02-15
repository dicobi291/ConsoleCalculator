#include "../header/Calculator.h"
#include "TestCalculator.h"

#include <iostream>
#include <cassert>

static Calculator calculator;

void testOperationSum()
{
    {
        //1 + 1 = 2
        Value val_1{{1}, {0}, false};
        Value val_2{{1}, {0}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{2}, {}, false};
        assert(res == expected);
    }
    {
        //0 + 2.5 = 2.5
        Value val_1{{0}, {}, false};
        Value val_2{{2}, {5}, false};

        auto res = calculator.sum(val_1, val_2);

        Value expected{{2}, {5}, false};
        assert(res == expected);
    }
    {
        //0 + 0 = 0
        Value val_1{{0}, {0}, false};
        Value val_2{{0}, {0}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //3 + 0 = 3
        Value val_1{{3}, {0}, false};
        Value val_2{{0}, {0}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{3}, {}, false};
        assert(res == expected);
    }
    {
        //0 + 3 = 3
        Value val_1{{0}, {0}, false};
        Value val_2{{3}, {0}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{3}, {}, false};
        assert(res == expected);
    }
    {
        //3 + 3 = 6
        Value val_1{{3}, {0}, true};
        Value val_2{{3}, {0}, true};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{6}, {}, true};
        assert(res == expected);
    }
    {
        //3 + (-3) = 0
        Value val_1{{3}, {0}, false};
        Value val_2{{3}, {0}, true};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{0}, {}, false};
        assert(res == expected);
    }

    {
        //3.34 + 6.66 = 10
        Value val_1{{3}, {3, 4}, false};
        Value val_2{{6}, {6, 6}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{1, 0}, {}, false};
        assert(res == expected);
    }
    {
        //209.2 + 1034.81 = 1244.01
        Value val_1{{2, 0, 9}, {2}, false};
        Value val_2{{1, 0, 3, 4}, {8, 1}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{1, 2, 4, 4}, {0, 1}, false};
        assert(res == expected);
    }
    {
        //20000 + 10000 = 30000
        Value val_1{{2, 0, 0, 0, 0}, {}, false};
        Value val_2{{1, 0, 0, 0, 0}, {}, false};

        auto res = calculator.sum(val_1, val_2);
        
        Value expected{{3, 0, 0, 0, 0}, {}, false};
        assert(res == expected);
    }
    std::cout << "testOperationSum OK\n";
}

void testOperationDiff()
{
    {
        //1 - 1 = 0
        Value val_1{{1}, {}, false};
        Value val_2{{1}, {}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //1 - 0 = 1
        Value val_1{{1}, {}, false};
        Value val_2{{0}, {}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{1}, {}, false};
        assert(res == expected);
    }
    {
        //0 - 1 = -1
        Value val_1{{0}, {}, false};
        Value val_2{{1}, {}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{1}, {}, true};
        assert(res == expected);
    }
    {
        //0 - 2.5 = -2.5
        Value val_1{{0}, {}, false};
        Value val_2{{2}, {5}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{2}, {5}, true};
        assert(res == expected);
    }
    {
        //0 - 0 = 0
        Value val_1{{0}, {}, false};
        Value val_2{{0}, {}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //1 - (-1) = 2
        Value val_1{{1}, {}, false};
        Value val_2{{1}, {}, true};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{2}, {}, false};
        assert(res == expected);
    }
    {
        //(-250) - (-750) = 500
        Value val_1{{2, 5, 0}, {}, true};
        Value val_2{{7, 5, 0}, {}, true};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{5, 0, 0}, {}, false};
        assert(res == expected);
    }
    {
        //(-250) - 750 = 1000
        Value val_1{{2, 5, 0}, {}, true};
        Value val_2{{7, 5, 0}, {}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{1, 0, 0, 0}, {}, true};
        assert(res == expected);
    }
    {
        //1 - 0.01 = 0.99
        Value val_1{{1}, {}, false};
        Value val_2{{0}, {0, 1}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{0}, {9, 9}, false};
        assert(res == expected);
    }
    {
        //2.5 - (-2.5) = 5
        Value val_1{{2}, {5}, false};
        Value val_2{{2}, {5}, true};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{5}, {}, false};
        assert(res == expected);
    }
    {
        //200.7 - 206.67 = 2.03
        Value val_1{{2, 0, 8}, {7}, false};
        Value val_2{{2, 0, 6}, {6, 7}, false};

        auto res = calculator.diff(val_1, val_2);

        Value expected{{2}, {0, 3}, false};
        assert(res == expected);
    }
    std::cout << "testOperationDiff OK\n";
}

void testOperationMulti()
{
    {
        //2 * 2 = 4
        Value val_1{{2}, {}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.multiple(val_1, val_2);

        Value expected{{4}, {}, false};
        assert(res == expected);
    }
    {
        //0 * 2 = 0
        Value val_1{{0}, {}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.multiple(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //2 * 0 = 0
        Value val_1{{2}, {}, false};
        Value val_2{{0}, {}, false};

        auto res = calculator.multiple(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //0 * 0 = 0
        Value val_1{{0}, {}, false};
        Value val_2{{0}, {}, false};

        auto res = calculator.multiple(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //2.5 * 0 = 0
        Value val_1{{2}, {5}, false};
        Value val_2{{0}, {}, false};

        auto res = calculator.multiple(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //-4687.57 * 1344 = -6300094.08
        Value val_1{{4, 6, 8, 7}, {5, 7}, true};
        Value val_2{{1, 3, 4, 4}, {}, false};

        auto res = calculator.multiple(val_1, val_2);

        Value expected{{6, 3, 0, 0, 0, 9, 4}, {0, 8}, true};
        assert(res == expected);
    }
    std::cout << "testOperationMulti OK\n";
}

void testOperationDivide()
{
    {
        //1.5 / 3 = 0.5
        Value val_1{{1}, {5}, false};
        Value val_2{{3}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{0}, {5}, false};
        assert(res == expected);
    }
    {
        //1.5 / 2 = 0.75
        Value val_1{{1}, {5}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{0}, {7, 5}, false};
        assert(res == expected);
    }
    {
        //2540 / 3 = 846.66
        Value val_1{{2, 5, 4, 0}, {}, false};
        Value val_2{{3}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{8, 4, 6}, {6, 6}, false};
        assert(res == expected);
    }
    {
        //2540 / 24 = 105.83
        Value val_1{{2, 5, 4, 0}, {}, false};
        Value val_2{{2, 4}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{1, 0, 5}, {8, 3}, false};
        assert(res == expected);
    }
    {
        //2540.5 / 25 = 101.62
        Value val_1{{2, 5, 4, 0}, {5}, false};
        Value val_2{{2, 5}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{1, 0, 1}, {6, 2}, false};
        assert(res == expected);
    }
    {
        //2540 / 25 = 101.6
        Value val_1{{2, 5, 4, 0}, {}, false};
        Value val_2{{2, 5}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{1, 0, 1}, {6}, false};
        assert(res == expected);
    }
    {
        //0 / 2 = 0
        Value val_1{{0}, {}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //0 / 2.5 = 0
        Value val_1{{0}, {}, false};
        Value val_2{{2}, {5}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{0}, {}, false};
        assert(res == expected);
    }
    {
        //2 / 2 = 1
        Value val_1{{2}, {}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{1}, {}, false};
        assert(res == expected);
    }
    {
        //44 / 2 = 22
        Value val_1{{4, 4}, {}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{2, 2}, {}, false};
        assert(res == expected);
    }
    {
        //25 / 2 = 12.5
        Value val_1{{2, 5}, {}, false};
        Value val_2{{2}, {}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{1, 2}, {5}, false};
        assert(res == expected);
    }    
    {
        //200.7 / 13.4 = 14.97
        Value val_1{{2, 0, 0}, {7}, false};
        Value val_2{{1, 3}, {4}, false};

        auto res = calculator.divide(val_1, val_2);

        Value expected{{1, 4}, {9, 7}, false};
        assert(res == expected);
    }
    std::cout << "testOperationDivide OK\n";
}

void allCalculatorTest()
{
    testOperationSum();
    testOperationDiff();
    testOperationMulti();
    testOperationDivide();
    
    std::cout << "allCalculatorTest OK\n";
}