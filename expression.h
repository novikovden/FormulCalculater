#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "operator.h"
#include <string>
#include "value.h"

class Expression: public Value {
public:
    Expression();
    Expression(Value *lhs, Value *rhs, Operator *op);
    virtual ~Expression();

    Expression& setLhs(Value *newlhs);
    Expression& setRhs(Value *newrhs);
    Expression& setOperands(Value *newlhs, Value *newrhs);
    Expression& setOperators(Operator *newop);

    virtual double calculate() const override;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual Expression* clone() const override;
    virtual InversionValue* inversion() override;
private:
    Value *_lhs, *_rhs;
    Operator *_operator;
};

#endif // EXPRESSION_H
