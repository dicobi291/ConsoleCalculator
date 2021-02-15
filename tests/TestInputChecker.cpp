#include "../header/InputChecker.h"
#include "TestInputChecker.h"

#include <iostream>
#include <cassert>

static InputChecker input_checker;

void testInputCheck()
{
    {
        std::string input {"2 + 2 * 2"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"2 + -2"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"2 +(-2)"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"(2 +(-2)"};

        auto res = input_checker.check(input);

        assert(res == false);
    }
    {
        std::string input {"(2 +(-2))"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"(2 +*(-2))"};

        auto res = input_checker.check(input);

        assert(res == false);
    }
    {
        std::string input {"(2 + (-2))/4 + 3"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"(2 + (-2))/4 + 3"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"00.2 - 1"};

        auto res = input_checker.check(input);

        assert(res == false);
    }
    {
        std::string input {"0.02 - 1"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"0.0.2 - 1"};

        auto res = input_checker.check(input);

        assert(res == false);
    }
    {
        std::string input {"2 +-2"};

        auto res = input_checker.check(input);

        assert(res == false);
    }
    {
        std::string input {"20000 + 300000"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"0/30.7"};

        auto res = input_checker.check(input);

        assert(res == true);
    }
    {
        std::string input {"200 * (300000 - 1.1)"};

        auto res = input_checker.check(input);

        assert(res == true);
    }

    std::cout << "testInputCheck OK\n";
}