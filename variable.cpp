#include "variable.h"

Variable::Variable(): Digit(), _name("") {}
Variable::Variable(std::string const &name): Digit(), _name(name) {}
Variable::Variable(std::string const &name, double value): Digit(value), _name(name) {}

std::ostream& Variable::print(std::ostream &os, std::string const &tab) const {
    return Digit::print(os << _name << " = ", tab);
}
Variable* Variable::clone() const {
    return new Variable(_name, _value);
}

Variable& Variable::setName(std::string const &newname) {
    _name = newname;
    return *this;
}
std::string Variable::getName() const {
    return _name;
}
