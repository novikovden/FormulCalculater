#include <algorithm>
#include <cctype>
#include "function.h"
#include "globalscope.h"
#include <sstream>
#include "variable.h"

template<typename T> std::string to_string(T const &t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
std::string tolower(std::string const &s) {
    std::string result;
    result.reserve(s.size());
    for(auto b = s.begin(), e = s.end(); b != e; ++b) {
        result.push_back(tolower(*b));
    }
    return result;
}

Scope::Scope() {}
Scope::~Scope() {}

GlobalScope::GlobalScope() {}
GlobalScope::~GlobalScope() {
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        delete *b;
    }
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        delete *b;
    }
}

GlobalScope& GlobalScope::defineFunction(AbstructFunction *newfun, bool *redefined) {
    if(redefined)
        *redefined = false;
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        if((*b)->getName() == newfun->getName() && (*b)->getCArg() == newfun->getCArg()) {
            delete *b;
            *b = newfun;
            if(redefined)
                *redefined = true;
            return *this;
        }
    }
    _functions.push_back(newfun);
    _sortFunctions();
    return *this;
}
GlobalScope& GlobalScope::defineVariable(Variable *newvar, bool *redefined) {
    if(redefined)
        *redefined = false;
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        if((*b)->getName() == newvar->getName()) {
            delete *b;
            *b = newvar;
            if(redefined)
                *redefined = true;
            return *this;
        }
    }
    _variables.push_back(newvar);
    _sortVariables();
    return *this;
}

bool GlobalScope::deleteFunction(AbstructFunction const *fun) {
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        if(*b == fun) {
            delete *b;
            _functions.erase(b);
            return true;
        }
    }
    return false;
}
bool GlobalScope::deleteFunction(std::string const &name, int carg) {
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        if((*b)->getName() == name && (*b)->getCArg() == carg) {
            delete *b;
            _functions.erase(b);
            return true;
        }
    }
    return false;
}
bool GlobalScope::deleteVariable(Variable const *variable) {
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        if(*b == variable) {
            delete *b;
            _variables.erase(b);
            return true;
        }
    }
    return false;
}
bool GlobalScope::deleteVariable(std::string const &name) {
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        if((*b)->getName() == name) {
            delete *b;
            _variables.erase(b);
            return true;
        }
    }
    return false;
}

AbstructFunction* GlobalScope::getFunction(std::string const &name, int carg) const {
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        if((*b)->getName() == name && (*b)->getCArg() == carg)
            return *b;
    }
    return nullptr;
}
Variable* GlobalScope::getVariable(std::string const &name) const {
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        if((*b)->getName() == name)
            return *b;
    }
    return nullptr;
}

std::string GlobalScope::getViewFunction(std::string const &name, int carg) const {
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        if((*b)->getName() == name && (*b)->getCArg() == carg) {
            return (*b)->getView();
        }
    }
    return std::string("");
}
std::string GlobalScope::getViewVariable(std::string const &name) const {
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        if((*b)->getName() == name) {
            return name + " = " + to_string((*b)->calculate());
        }
    }
}

std::vector<std::string> GlobalScope::getNamesFunctions() const {
    std::vector<std::string> result;
    result.reserve(_functions.size());
    std::string buffer;
    for(auto b = _functions.begin(), e = _functions.end(); b != e; ++b) {
        buffer = (*b)->getName() + "(";
        std::vector<std::string> args = (*b)->getArguments();
        if(!args.empty()) {
            buffer += args.front();
            for(auto b = args.begin()+1, e = args.end(); b != e; ++b) {
                buffer += std::string(", ")+(*b);
            }
        }
        buffer += ")";
        result.push_back(buffer);
    }
    return result;
}
std::vector<std::string> GlobalScope::getNamesVariables() const {
    std::vector<std::string> result;
    result.reserve(_variables.size());
    for(auto b = _variables.begin(), e = _variables.end(); b != e; ++b) {
        result.push_back((*b)->getName());
    }
    return result;
}

void GlobalScope::_sortFunctions() {
    std::sort(_functions.begin(), _functions.end(), [](AbstructFunction const *lhs, AbstructFunction const *rhs)->bool{
        if(lhs->getName() < rhs->getName())
            return true;
        if(lhs->getName() == rhs->getName()) {
            return lhs->getCArg() < rhs->getCArg();
        }
        return false;
    });
}
void GlobalScope::_sortVariables() {
    std::sort(_variables.begin(), _variables.end(), [](Variable const *lhs, Variable const *rhs)->bool{
        return tolower(lhs->getName()) < tolower(rhs->getName());
    });
}

// END
