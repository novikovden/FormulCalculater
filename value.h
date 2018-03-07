#ifndef VALUE_H
#define VALUE_H

#include <iostream>

class Value {
public:
    Value();
    virtual ~Value();

    virtual double calculate() const = 0;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const = 0;
    virtual Value* clone() const = 0;
    virtual Value* inversion();
};

class InversionValue: public Value {
public:
    InversionValue();
    InversionValue(Value *value);
    virtual ~InversionValue();

    virtual double calculate() const override;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual InversionValue* clone() const override;
    virtual Value* inversion() override;
private:
    Value *_value;
};

#endif // VALUE_H
