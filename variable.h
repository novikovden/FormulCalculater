#ifndef VARIABLE_H
#define VARIABLE_H

#include "digit.h"

class Variable: public Digit {
public:
    Variable();
    Variable(std::string const &name);
    Variable(std::string const &name, double value);

    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual Variable* clone() const override;

    Variable& setName(std::string const &newname);
    std::string getName() const;
private:
    std::string _name;
};

#endif // VARIABLE_H
