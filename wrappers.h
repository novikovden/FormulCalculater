#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <string>
#include <vector>

class Value;
class Scope;
class FunctionWrapper: public Value {
public:
    FunctionWrapper(Scope const *scope = 0);
    FunctionWrapper(std::string const &name, std::vector<Value*> const &arguments);
    virtual ~FunctionWrapper();

    virtual double calculate() const override;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual FunctionWrapper* clone() const override;

    FunctionWrapper& setScope(Scope const *newscope);
    FunctionWrapper& setName(std::string const &newname);
    FunctionWrapper& setArguments(std::vector<Value*> const &newarg);

    Scope const* getScope() const;
    std::string getName() const;
    std::vector<Value*> getArguments() const;
    int getCArg() const;
    bool test() const;
private:
    Scope const *_scope;
    std::string _name;
    std::vector<Value*> _arguments;
};

class VariableWrapper: public Value {
public:
    VariableWrapper(Scope const *scope = 0);
    VariableWrapper(std::string const &name);
    VariableWrapper(Scope const *scope, std::string const &name);
    virtual ~VariableWrapper();

    virtual double calculate() const override;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual VariableWrapper* clone() const override;

    VariableWrapper& setScope(Scope const *newscope);
    VariableWrapper& setName(std::string const &newname);

    Scope const* getScope() const;
    std::string getName() const;
    bool test() const;
private:
    Scope const *_scope;
    std::string _name;
};

class ValueWrapper: public Value {
public:
    ValueWrapper(Value const *pointer = nullptr);
    virtual ~ValueWrapper();

    virtual double calculate() const override;
    virtual std::ostream& print(std::ostream &os, std::string const &tab) const override;
    virtual ValueWrapper* clone() const override;

    ValueWrapper& setValue(Value const *newpointer);
    Value const* getValue() const;
private:
    Value const *_pointer;
};

#endif // WRAPPER_H
