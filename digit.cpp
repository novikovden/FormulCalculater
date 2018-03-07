#include "digit.h"

Digit::Digit(double value): _value(value) {}
Digit::~Digit() {}

double Digit::calculate() const {
    return _value;
}
std::ostream& Digit::print(std::ostream &os, std::string const &tab) const {
    return os << _value;
}
Digit* Digit::clone() const {
    return new Digit(_value);
}
Digit* Digit::inversion() {
    _value = -_value;
    return this;
}


// END
