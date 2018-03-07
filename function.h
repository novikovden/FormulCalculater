#ifndef FUNCTION_H
#define FUNCTION_H

#include "exception.h"
#include <functional>
#include <string>
#include <vector>

template<typename T, typename V> struct Pair {
    Pair() {}
    Pair(T const &t, V const &v): first(t), second(v) {}

    T first;
    V second;
};


class Scope;
class Operator;
class Value;
class ValueWrapper;

class AbstructFunction {
public:
    AbstructFunction();
    virtual ~AbstructFunction();

    virtual std::string getName() const = 0;
    virtual std::vector<std::string> getArguments() const = 0;
    virtual int getCArg() const = 0;
    virtual std::string getView() const = 0;

    virtual double calculate(std::vector<Value*> const &arguments) const = 0;
};

class Function: public AbstructFunction {
public:
    typedef std::string::const_iterator iterator;

    Function(Scope const *scope = nullptr);
    virtual ~Function();

    Function& setScope(Scope const *newscope);
    Function& setName(std::string const &newname) throw(ExplanatoryException);
    Function& setArgsBody(std::vector<std::string> const &newargs, std::string const &newbody) throw (ExplanatoryException);
    Function& setBody(std::string const &newbody) throw (ExplanatoryException);

    virtual std::string getName() const override;
    virtual std::vector<std::string> getArguments() const override;
    virtual int getCArg() const override;
    std::string getBody() const;
    virtual std::string getView() const override;

    virtual double calculate(std::vector<Value*> const &arguments) const override;

private:
    Value* _extractExpression(iterator &b, iterator e);
    Value* _extractOperand(iterator &b, iterator e);
    Operator* _extractOperator(iterator &b, iterator e);


    Scope const *_scope;

    std::string _name;
    std::vector<std::string> _arguments;
    std::vector< Pair<int, ValueWrapper*> > _argsvalue;
    std::string _body;

    Value *_expression;

    Function(Function const &f);
    Function& operator=(Function const &f);

};

class ConcreteFunction: public AbstructFunction {
public:
    ConcreteFunction(std::string const &name = "");
    ConcreteFunction(std::string const &name, std::function<double(std::vector<Value*> const &)> function);
    virtual ~ConcreteFunction();

    ConcreteFunction& setName(std::string const &newname);
    ConcreteFunction& setArguments(std::vector<std::string> const &newargs);

    virtual std::string getName() const override;
    virtual std::vector<std::string> getArguments() const override;
    virtual int getCArg() const override;
    virtual std::string getView() const override;

    virtual double calculate(std::vector<Value*> const &arguments) const;

private:
    std::string _name;
    std::vector<std::string> _arguments;
    std::function<double(std::vector<Value*> const &)> _function;
};

#endif // FUNCTION_H
