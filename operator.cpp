#include "operator.h"

Operator::Operator(): _view("x"), _op([](double, double)->double{return 0;}), _priority(0) {}
Operator::Operator(std::function<double(double, double)> const &op): _view("x"), _op(op), _priority(0) {}
Operator::Operator(std::string const &view, std::function<double(double, double)> const &op, int priority):
    _view(view), _op(op), _priority(priority) {}
Operator::~Operator() {}

Operator& Operator::setView(std::string const &newview) {
    _view = newview;
    return *this;
}
Operator& Operator::setPriority(int newpriority) {
    _priority = newpriority;
    return *this;
}
Operator& Operator::setOperation(std::function<double(double, double)> const &newop) {
    _op = newop;
    return *this;
}

int Operator::getPriority() const {
    return _priority;
}
double Operator::work(double lhs, double rhs) const {
    return _op(lhs, rhs);
}
std::string Operator::getView() const {
    return _view;
}

// END
