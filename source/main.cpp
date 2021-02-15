#include "../header/stdafx.h"
#include "../header/InputChecker.h"
#include "../header/Calculator.h"
#include "../header/Evaluator.h"
#include "../tests/TestCalculator.h"
#include "../tests/TestInputChecker.h"

int main(int argc, char *argv[])
{
    // Uncomment for test
    // allCalculatorTest();
    // testInputCheck();
    // return 0;

    if (argc > 1) {
        if (!argv[1]) {
            std::cout << "Usage: ./calculator \"%ARITHMETIC_EXPRESSION%\"\n";
            std::cout << "For example ./calculator \"2 + 2 * 2\"\n";
            return 0;
        }
        std::string input {argv[1]};
        InputChecker input_checker;
        if (!input_checker.check(input)) {
            std::cout << input_checker.getLastError();
            std::cout << input << std::endl;
            for (int i = 0; i < input_checker.getErrorIndex(); ++i) {
                std::cout << " ";
            }
            std::cout << '^' << std::endl;
            return 0;
        }
        std::cout << input;
        Evaluator evaluator;
        try {
            auto res = evaluator.evaluate(input);
            std::cout << " = ";
            printValue(res);
        } catch (const std::exception &e) {
            std::cout << "\nError: " << e.what() << std::endl;
            return 0;
        }
    } else {
        std::cout << "Usage: ./calculator \"%ARITHMETIC_EXPRESSION%\"\n";
        std::cout << "For example ./calculator \"2 + 2 * 2\"\n";
    }

    return 0;
}