#include "expression.h"

Expression::Expression(): _lhs(nullptr), _rhs(nullptr), _operator(nullptr) {}
Expression::Expression(Value* lhs, Value *rhs, Operator *op):
    _lhs(lhs), _rhs(rhs), _operator(op) {}
Expression::~Expression() {
    if(_lhs) delete _lhs;
    if(_rhs) delete _rhs;
    if(_operator) delete _operator;
}

Expression& Expression::setLhs(Value *newlhs) {
    if(_lhs) delete _lhs;
    _lhs = newlhs;
    return *this;
}
Expression& Expression::setRhs(Value *newrhs) {
    if(_rhs) delete _rhs;
    _rhs = newrhs;
    return *this;
}
Expression& Expression::setOperands(Value *newlhs, Value *newrhs) {
    if(_lhs) delete _lhs;
    _lhs = newlhs;
    if(_rhs) delete _rhs;
    _rhs = newrhs;
    return *this;
}
Expression& Expression::setOperators(Operator *newop) {
    if(_operator) delete _operator;
    _operator = newop;
    return *this;
}

double Expression::calculate() const {
    return _operator->work(_lhs->calculate(), _rhs->calculate());
}
std::ostream& Expression::print(std::ostream &os, std::string const &tab) const {
    std::string edittab = tab+"\t";
    os << _operator->getView() << "{\n";
    _lhs->print(os << edittab, edittab) << "\n";
    _rhs->print(os << edittab, edittab) << "\n";
    return os << tab << "} = " << calculate();
}
Expression* Expression::clone() const {
    return new Expression(_lhs->clone(), _rhs->clone(), new Operator(*_operator));
}
InversionValue* Expression::inversion() {
    return new InversionValue(this);
}

// END
