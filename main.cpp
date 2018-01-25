#include "digit.h"
#include "expression.h"
#include <iostream>
#include "operator.h"

using namespace std;


int main() {
    auto addition = [](double lhs, double rhs)->double {
        return lhs+rhs;
    };
    auto subtraction = [](double lhs, double rhs)->double {
        return lhs-rhs;
    };
    auto multiplication = [](double lhs, double rhs)->double {
        return lhs*rhs;
    };
    auto devision = [](double lhs, double rhs)->double {
        return lhs/rhs;
    };

    int counter = 0;
    while(true) {
        ++counter;
        Value *value = new Expression(
            new Expression(
                new InversionValue(new Digit(counter)),
                new Expression(
                    new Digit(100),
                    new Digit(5),
                    new Operator("/", devision)
                ),
                new Operator("+", addition)
            ),
            new Expression(
                new Digit(10),
                new Digit(6),
                new Operator("-", subtraction)
            ),
            new Operator("*", multiplication)
        );
        value->print(cout, "");
        delete value;
    }

    return 0;
}
