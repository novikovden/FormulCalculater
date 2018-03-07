#include "exception.h"
#include "function.h"
#include "globalscope.h"
#include <sstream>
#include <string>
#include "variable.h"
#include "wrappers.h"

template<typename T> std::string to_string(T const &t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

FunctionWrapper::FunctionWrapper(Scope const *parent): _scope(parent) {}
FunctionWrapper::FunctionWrapper(std::string const &name, std::vector<Value*> const &arguments): _scope(nullptr), _name(name) {
    _arguments.reserve(arguments.size());
    for(auto b = arguments.cbegin(), e = arguments.cend(); b != e; ++b) {
        _arguments.push_back(*b);
    }
}
FunctionWrapper::~FunctionWrapper() {
    for(auto b = _arguments.begin(), e = _arguments.end(); b != e; ++b) {
        delete *b;
    }
}

double FunctionWrapper::calculate() const {
    if(!_scope)
        throw ExceptionOneMessage("Use FunctionWrapper without parent");
    auto *f = _scope->getFunction(_name, _arguments.size());
    if(!f)
        throw ExceptionOneMessage(std::string("Function not find - ") + _name + "(" + to_string(_arguments.size()) + ")");
    return f->calculate(_arguments);
}
std::ostream& FunctionWrapper::print(std::ostream &os, std::string const &tab) const {
    return os << _name << "(" << _arguments.size() << ") = " << calculate();
}
FunctionWrapper* FunctionWrapper::clone() const {
    std::vector<Value*> cpargs;
    cpargs.reserve(_arguments.size());
    for(auto b = _arguments.begin(), e = _arguments.end(); b != e; ++b) {
        cpargs.push_back((*b)->clone());
    }

    FunctionWrapper *f = new FunctionWrapper(_name, cpargs);
    f->setScope(_scope);
    return f;
}

FunctionWrapper& FunctionWrapper::setScope(Scope const *newparent) {
    _scope = newparent;
    return *this;
}
FunctionWrapper& FunctionWrapper::setName(std::string const &newname) {
    _name = newname;
    return *this;
}
FunctionWrapper& FunctionWrapper::setArguments(std::vector<Value*> const &newargs) {
    for(auto b = _arguments.begin(), e = _arguments.end(); b != e; ++b) {
        delete *b;
    }
    _arguments.clear();
    _arguments.reserve(newargs.size());
    for(auto b = newargs.begin(), e = newargs.end(); b != e; ++b) {
        _arguments.push_back(*b);
    }
    return *this;
}

std::string FunctionWrapper::getName() const {
    return _name;
}
std::vector<Value*> FunctionWrapper::getArguments() const {
    return _arguments;
}
int FunctionWrapper::getCArg() const {
    return _arguments.size();
}
bool FunctionWrapper::test() const {
    if(!_scope)
        throw ExceptionOneMessage("Use FunctionWrapper without parent");
    return _scope->getFunction(_name, _arguments.size());
}


VariableWrapper::VariableWrapper(Scope const *parent): _scope(parent) {}
VariableWrapper::VariableWrapper(std::string const &name): _name(name) {}
VariableWrapper::VariableWrapper(Scope const *parent, std::string const &name): _scope(parent), _name(name) {}
VariableWrapper::~VariableWrapper() {}

double VariableWrapper::calculate() const {
    if(!_scope)
        throw ExceptionOneMessage("Use VariableWrapper without parent");
    Variable* var = _scope->getVariable(_name);
    if(!var)
        throw ExceptionOneMessage(std::string("Variable not find - '") + _name + std::string("'"));
    return var->calculate();
}
std::ostream& VariableWrapper::print(std::ostream &os, std::string const &tab) const {
    return os << _name << " = " << calculate();
}
VariableWrapper* VariableWrapper::clone() const {
    return new VariableWrapper(_scope, _name);
}

VariableWrapper& VariableWrapper::setScope(Scope const *newparent) {
    _scope = newparent;
    return *this;
}
VariableWrapper& VariableWrapper::setName(std::string const &newname) {
    _name = newname;
    return *this;
}

Scope const* VariableWrapper::getScope() const {
    return _scope;
}
std::string VariableWrapper::getName() const {
    return _name;
}
bool VariableWrapper::test() const {
    if(!_scope)
        throw ExceptionOneMessage("Use VariableWrapper without parent");
    return _scope->getVariable(_name);
}

ValueWrapper::ValueWrapper(Value const *pointer): _pointer(pointer) {}
ValueWrapper::~ValueWrapper() {}

double ValueWrapper::calculate() const {
    if(!_pointer)
        throw ExceptionOneMessage("Use not defined ValueWrapper");
    return _pointer->calculate();
}
std::ostream& ValueWrapper::print(std::ostream &os, std::string const &tab) const {
    if(!_pointer)
        throw ExceptionOneMessage("Use not defined ValueWrapper");
    return _pointer->print(os << "&", tab);
}
ValueWrapper* ValueWrapper::clone() const {
    return new ValueWrapper(_pointer);
}

ValueWrapper& ValueWrapper::setValue(Value const *newpointer) {
    _pointer = newpointer;
}
Value const* ValueWrapper::getValue() const {
    return _pointer;
}

// END
