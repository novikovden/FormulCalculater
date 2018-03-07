#ifndef OPERATOR_H
#define OPERATOR_H

#include <functional>
#include <string>

class Operator {
public:
    Operator();
    Operator(std::function<double(double, double)> const &op);
    Operator(std::string const &view, std::function<double(double, double)> const &op, int priority = 0);
    ~Operator();

    Operator& setView(std::string const &newview);
    Operator& setPriority(int newpriority);
    Operator& setOperation(std::function<double(double, double)> const &newop);

    int getPriority() const;
    double work(double lhs, double rhs) const;
    std::string getView() const;
private:
    std::string _view;
    std::function<double(double, double)> _op;
    int _priority;
};

#endif // OPERATOR_H
