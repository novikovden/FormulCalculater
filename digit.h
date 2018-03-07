#ifndef DIGIT_H
#define DIGIT_H

#include <string>
#include "value.h"

class Digit: public Value {
public:
    Digit(double value = 0);
    virtual ~Digit();

    virtual double calculate() const override;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual Digit* clone() const override;
    virtual Digit* inversion() override;
protected:
    double _value;
};

#endif // DIGIT_H
