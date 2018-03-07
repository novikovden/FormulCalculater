#include <cmath>
#include "digit.h"
#include "exception.h"
#include "expression.h"
#include "function.h"
#include <functional>
#include "globalscope.h"
#include "helpthings.h"
#include "operator.h"
#include "value.h"
#include "wrappers.h"

AbstructFunction::AbstructFunction() {}
AbstructFunction::~AbstructFunction() {}

Function::Function(Scope const *scope): _scope(scope), _expression(nullptr) {}
Function::~Function() {
    if(_expression)
        delete _expression;
}

Function& Function::setScope(Scope const *newscope) {
    _scope = newscope;
    return *this;
}
Function& Function::setName(std::string const &newname) throw(ExplanatoryException) {
    _name = newname;
    return *this;
}
Function& Function::setArgsBody(std::vector<std::string> const &newargs, std::string const &newbody) throw(ExplanatoryException) {
    _arguments = newargs;
    return setBody(newbody);
}
Function& Function::setBody(std::string const &newbody) throw(ExplanatoryException) {
    std::string reservebody = _body;
    Value *resexpr = _expression;
    _body = newbody;
    iterator b = _body.begin();
    try {
        _expression = _extractExpression(b, _body.cend());
        if(b != _body.end())
            throw 1;
    }
    catch(ExplanatoryException &e) {
        _body = reservebody;
        _expression = resexpr;
        throw e;
    }
    catch(Expression &e) {
        _body = reservebody;
        _expression = resexpr;
        throw e;
    }
    catch(...) {
        _body = reservebody;
        _expression = resexpr;
        throw;
    }

    return *this;
}

std::string Function::getName() const {
    return _name;
}
std::vector<std::string> Function::getArguments() const {
    return _arguments;
}
int Function::getCArg() const {
    return _arguments.size();
}
std::string Function::getView() const {
    std::string result = _name + "(";
    if(!_arguments.empty()) {
        result += _arguments.front();
        for(auto b = _arguments.begin()+1, e = _arguments.end(); b != e; ++b) {
            result += std::string(", ")+(*b);
        }
    }
    result += ") = " + _body;
    return result;
}
double Function::calculate(std::vector<Value*> const &arguments) const {
    if(!_expression)
        throw ExceptionOneMessage(std::string("Use undefined function - ") + _name);
    if(_arguments.size() != arguments.size())
        throw 1;
    for(auto b = _argsvalue.begin(), e = _argsvalue.end(); b != e; ++b) {
        b->second->setValue(arguments[b->first]);
    }
    return _expression->calculate();
}


Value* Function::_extractExpression(iterator &b, iterator e) {
    std::vector<Value*> operands;
    std::vector<Operator*> operators;

    Operator *bufopr = nullptr;

    auto clear = [&operands, &operators]()->void {
        for(auto b = operands.begin(), e = operands.end(); b != e; ++b) {
            if(*b)
                delete *b;
        }
        for(auto b = operators.begin(), e = operators.end(); b != e; ++b) {
            if(*b)
                delete *b;
        }
    };

    try {
        while(true) {
            operands.push_back(_extractOperand(b, e));
            if(b == e)
                break;
            bufopr = _extractOperator(b, e);
            while(!operators.empty() && bufopr->getPriority() <= operators.back()->getPriority()) {
                Expression* expr = new Expression(*(operands.end()-2), *(operands.end()-1), operators.back());
                operands.pop_back();
                operands.back() = expr;
                operators.pop_back();
            }
            operators.push_back(bufopr);
        }
        while(!operators.empty()) {
            Expression *expr = new Expression(*(operands.end()-2), *(operands.end()-1), operators.back());
            operators.pop_back();
            operands.pop_back();
            operands.back() = expr;
        }
        return operands.back();
    }
    catch(ExplanatoryException &e) {
        clear();
        throw e;
    }
    catch(...) {
        clear();
        throw;
    }
    return nullptr;
}

Value* Function::_extractOperand(iterator &b, iterator e) {
    if(b == e)
        throw ExplanatoryException("Expected Operand", _body, b);
    if(isdigit(*b)) {
        std::string input;
        input.push_back(*(b++));
        bool wasPoint = false;
        while(b != e) {
            if(isdigit(*b)) {
                input.push_back(*b);
                wasPoint = false;
            } else if(*b == '.') {
                if(wasPoint)
                    throw ExplanatoryException("Invalid Operand", _body, b);
                wasPoint = true;
                input.push_back(*b);
            } else if(wasPoint)
                throw ExplanatoryException("Invalid Operand", _body, b);
            else break;
            ++b;
        }
        return new Digit(std::stod(input));
    }
    else if(*b == '-') {
        return _extractOperand(++b, e)->inversion();
    }
    else if(*b == '(') {
        ++b;
        iterator end = b;
        int brackets = 1;
        while(end != e) {
            if(*end == ')') {
                if(--brackets == 0) {
                    Value *result = _extractExpression(b, end);
                    b = end+1;
                    return result;
                }
            }
            else if(*end == '(')
                ++brackets;
            ++end;
        }
        throw ExplanatoryException("No pair bracket", _body, b-1);
    }
    else if(std::isalpha(*b)) { // It's Function or Variable
        iterator start = b;
        int pos = 0;
        std::string name = extractName(b, e, &pos);
        b += pos;
        if(*b == '(') { // It's Function
            ++b;
            int brackets = 1;
            std::vector<Value*> arguments;
            iterator end;
            if(*b != ')') while(true) {
                end = b;
                while(*end != ',') {
                    if(*end == '(')
                        ++brackets;
                    else if(*end == ')' && --brackets == 0)
                        break;
                    ++end;
                    if(end == e)
                        throw ExplanatoryException("No pair bracke1t", _body, b-1);
                }
                arguments.push_back(_extractExpression(b, end));
                if(*(b++) == ')')
                    break;
            }
            else
                ++b;
            FunctionWrapper *fw = new FunctionWrapper(_scope);
            fw->setName(name);
            fw->setArguments(arguments);
            if(!fw->test())
                throw ExplanatoryException("Function not Find", _body, start);
            return fw;
        }
        else { // It's Variable
            for(size_t i = 0; i != _arguments.size(); ++i) {
                if(_arguments[i] == name) {
                    _argsvalue.push_back( Pair<int, ValueWrapper*>(i, new ValueWrapper) );
                    return _argsvalue.back().second;
                }
            }
            VariableWrapper *vw = new VariableWrapper(_scope);
            vw->setName(name);
            if(!vw->test())
                throw ExplanatoryException("Variable not Find", _body, start);
            return vw;
        }
    }
    else
        throw ExplanatoryException("Expected Operand", _body, b);
}
Operator* Function::_extractOperator(iterator &b, iterator e) {
    std::function<double(double, double)> op;
    int priority = 0;
    if(b == e)
        throw ExplanatoryException("Expected Operator", _body, b);
    switch(*b) {
    case ')':
        return nullptr;
    case '+':
        op = [](double lhs, double rhs)->double {
            return lhs+rhs;
        };
        priority = 0;
        break;
    case '-':
        op = [](double lhs, double rhs)->double {
            return lhs-rhs;
        };
        priority = 0;
        break;
    case '*':
        op = [](double lhs, double rhs)->double {
            return lhs*rhs;
        };
        priority = 1;
        break;
    case '/': case ':':
        op = [](double lhs, double rhs)->double {
            return lhs/rhs;
        };
        priority = 1;
        break;
    case '^':
        op = [](double lhs, double rhs)->double {
            return std::pow(lhs, rhs);
        };
        priority = 2;
        break;
    default:
        throw ExplanatoryException("Expected Operator", _body, b);
    }
    return new Operator(std::to_string(*(b++)), op, priority);
}

ConcreteFunction::ConcreteFunction(std::string const &name): _name(name) {}
ConcreteFunction::ConcreteFunction(std::string const &name, std::function<double(std::vector<Value*> const &)> function):
    _name(name), _function(function) {}
ConcreteFunction::~ConcreteFunction() {}

ConcreteFunction& ConcreteFunction::setName(std::string const &newname) {
    _name = newname;
    return *this;
}
ConcreteFunction& ConcreteFunction::setArguments(std::vector<std::string> const &newargs) {
    _arguments = newargs;
    return *this;
}

std::string ConcreteFunction::getName() const {
    return _name;
}
std::vector<std::string> ConcreteFunction::getArguments() const {
    return _arguments;
}
int ConcreteFunction::getCArg() const {
    return _arguments.size();
}
std::string ConcreteFunction::getView() const {
    std::string result = _name + "(";
    if(!_arguments.empty()) {
        result += _arguments.front();
        for(auto b = _arguments.begin()+1, e = _arguments.end(); b != e; ++b) {
            result += ", " + *b;
        }
    }
    result += ")";
    return result;
}

double ConcreteFunction::calculate(std::vector<Value*> const &arguments) const {
    if(!_function)
        throw ExceptionOneMessage("Use not-defined ConcreteFunction");
    return _function(arguments);
}

// END
