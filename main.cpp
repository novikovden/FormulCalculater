#include <cmath>
#include "digit.h"
#include "expression.h"
#include "function.h"
#include "globalscope.h"
#include <iostream>
#include "operator.h"
#include <string>
#include "variable.h"
#include "wrappers.h"

/* Сделать:
    - Сократить код в main.cpp
*/

using namespace std;

void test_expression_tree() {
    auto addition = [](double lhs, double rhs)->double {
        return lhs+rhs;
    };
    auto subtraction = [](double lhs, double rhs)->double {
        return lhs-rhs;
    };
    auto multiplication = [](double lhs, double rhs)->double {
        return lhs*rhs;
    };
    auto devision = [](double lhs, double rhs)->double {
        return lhs/rhs;
    };

    Value *value = new Expression(
        new Expression(
            (new Digit(-100))->inversion(),
            new Expression(
                new Digit(100),
                new Digit(5),
                new Operator("/", devision)
            ),
            new Operator("+", addition)
        ),
        new Expression(
            new Digit(10),
            new Digit(6),
            new Operator("-", subtraction)
        ),
        new Operator("*", multiplication)
    );
    value->print(cout, "");
    delete value;
}

Function* createFunction(string const &name, vector<string> const &args, string const &body, GlobalScope const *parent) {
    Function *fun = new Function(parent);
    fun->setName(name);
    fun->setArgsBody(args, body);
    return fun;
}
Variable* createVariable(string const &name, double value) {
    return new Variable(name, value);
}

void print_variables_names(GlobalScope const &gs) {
    vector<string> v = gs.getNamesVariables();
    cout << "--------------------------------------------\n";
    for(auto b = v.begin(), e = v.end(); b != e; ++b) {
        cout << *b << endl;
    }
    cout << "--------------------------------------------\n";
}


string trim(string const &s) {
    string::size_type p = 0;
    while(p < s.size() && s[p] == ' ')
        ++p;

    string result = s.substr(p);
    for(int e = result.size()-1; e != 0; --e) {
        if(result[e] == ' ')
            result.pop_back();
        else
            break;
    }
    return result;
}
string getName(string::const_iterator &b, string::const_iterator e) {
    string result;
    if(isalpha(*b) || *b == '_')
        result.push_back(*b);
    else
        return "";
    ++b;
    while(b < e && (isdigit(*b) || isalpha(*b) || *b == '_'))
        result.push_back(*(b++));
    return result;
}
bool startedwith(string const &who, string const &with) {
    for(size_t i = 0; i < with.size(); ++i) {
        if(i >= who.size() || who[i] != with[i])
            return false;
    }
    return true;
}
string substr(string const &s, string::const_iterator from) {
    string result;
    for(; from != s.end(); ++from) {
        result.push_back(*from);
    }
    return result;
}
void define_default_function(GlobalScope &gs) {
    using Args = std::vector<Value*>;

    gs.defineFunction( &(new ConcreteFunction("pow", [](Args a)->double{
        return pow(a[0]->calculate(), 2);
    }))->setArguments({"x"}) );
    gs.defineFunction( &(new ConcreteFunction("pow", [](Args a)->double{
        return pow(a[0]->calculate(), a[1]->calculate());
    }))->setArguments({"x", "power"}) );
    gs.defineFunction( &(new ConcreteFunction("sqrt", [](Args a)->double{
        return sqrt(a[0]->calculate());
    }))->setArguments({"x"}) );
    gs.defineFunction( &(new ConcreteFunction("sqrt", [](Args a)->double{
        return pow(a[0]->calculate(), 1/a[1]->calculate());
    }))->setArguments({"x", "power"}) );

    gs.defineFunction( &(new ConcreteFunction("sin", [](Args a)->double{
        return sin(a[0]->calculate());
    }))->setArguments({"angle"}) );
    gs.defineFunction( &(new ConcreteFunction("cos", [](Args a)->double{
        return cos(a[0]->calculate());
    }))->setArguments({"angle"}) );
    gs.defineFunction( &(new ConcreteFunction("tg", [](Args a)->double{
        return tan(a[0]->calculate());
    }))->setArguments({"angle"}) );
    gs.defineFunction( &(new ConcreteFunction("ctg", [](Args a)->double{
        return 1/tan(a[0]->calculate());
    }))->setArguments({"angle"}) );

    gs.defineFunction( &(new ConcreteFunction("arcsin", [](Args a)->double{
        return asin(a[0]->calculate());
    }))->setArguments({"value"}) );
    gs.defineFunction( &(new ConcreteFunction("arccos", [](Args a)->double{
        return acos(a[0]->calculate());
    }))->setArguments({"value"}) );
    gs.defineFunction( &(new ConcreteFunction("arctg", [](Args a)->double{
        return atan(a[0]->calculate());
    }))->setArguments({"value"}) );
    gs.defineFunction( &(new ConcreteFunction("arcctg", [](Args a)->double{
        return atan(tan(a[0]->calculate()));
    }))->setArguments({"value"}) );
}
void define_default_variables(GlobalScope &gs) {
    gs.defineVariable(new Variable("pi", 3.1415926535 ));
    gs.defineVariable(new Variable("e", 2.7182818284 ));
    gs.defineVariable(new Variable("g", 9.8 ));
    gs.defineVariable(new Variable("G", 6.67408*pow(10, -11) ));
    gs.defineVariable(new Variable("c", 299792458 ));
}


int main() {
        GlobalScope globalscope;
        define_default_variables(globalscope);
        define_default_function(globalscope);

        string input;
        string name;
        string substring;
        string::const_iterator it;
        string::size_type p = 0;
        int pos = -1;
        double value;
        bool redefined;
        while(true) {
            try {
                cout << "> ";
                getline(cin, input);
                input = trim(input);
                if((startedwith(input, "define ") && (pos = 7)) || (startedwith(input, "def ") && (pos = 4))) {
                    it = input.begin()+pos;
                    while(*it == ' ' && it < input.end())
                        ++it;
                    name = getName(it, input.end());
                    if(name == "")
                        throw ExplanatoryException("Invalid name", input, it);
                    while(*it == ' ' && it < input.end())
                        ++it;

                    if(*it == '=') {
                        ++it;
                        while(*it == ' ' && it < input.end())
                            ++it;

                        if(!isdigit(*it) || it == input.end())
                            throw ExplanatoryException("Expected the Digit", input, it);

                        substring = substr(input, it);
                        value = stod(substring, &p);
                        if(p != substring.size())
                            throw ExplanatoryException("Unexpected character", input, it+p);
                        Variable *var = createVariable(name, value);
                        globalscope.defineVariable(var, &redefined);
                        if(redefined)
                            cout << "Redefined";
                        else
                            cout << "Defined";
                        cout << " Variable: " << var->getName() << " = " << var->calculate() << endl;
                    }
                    else if(*it == '(') {
                        ++it;
                        while(*it == ' ' && it < input.end())
                            ++it;
                        vector<string> arguments;
                        if(*it != ')') while(true) {
                            arguments.push_back(getName(it, input.end()));
                            if(arguments.back() == "")
                                throw ExplanatoryException("Expected argument's name", input, it);
                            while(*it == ' ' && it < input.end())
                                ++it;
                            if(*it == ',') {
                                ++it;
                                while(*it == ' ' && it < input.end())
                                    ++it;
                                continue;
                            }
                            if(*it == ')')
                                break;
                            throw ExplanatoryException("Expected end of arguments list or comma", input, it);
                        }
                        ++it;
                        while(*it == ' ' && it < input.end())
                            ++it;
                        if(*it != '=')
                            throw ExplanatoryException("Expected '='", input, it);
                        ++it;
                        while(*it == ' ' && it < input.end())
                            ++it;
                        substring = substr(input, it);
                        Function *fun = createFunction(name, arguments, substring, &globalscope);
                        globalscope.defineFunction(fun, &redefined);
                        if(redefined)
                            cout << "Redefined";
                        else
                            cout << "Defined";
                        cout << " Function - " << fun->getView() << endl;
                    }
                    else
                        throw ExplanatoryException("Expected '=' or '('", input, it);
                }
                else if(startedwith(input, "print ") && (pos = 6)) {
                    it = input.begin()+pos;
                    while(*it == ' ' && it < input.end())
                        ++it;
                    name = getName(it, input.end());
                    if(name == "")
                        throw ExplanatoryException("Expected name of function or variable", input, it);
                    while(*it == ' ' && it < input.end())
                        ++it;
                    if(it == input.end()) {
                        Variable *var = globalscope.getVariable(name);
                        if(var == nullptr)
                            cout << "Variable not Find" << endl;
                        else
                            cout << var->getName() << " = " << var->calculate() << endl;
                    }
                    else if(*it == '(') {
                        int val = 0;
                        ++it;
                        while(*it == ' ' && it < input.end())
                            ++it;
                        vector<string> arguments;
                        if(*it != ')') while(true) {
                            arguments.push_back(getName(it, input.end()));
                            if(arguments.back() == "") {
                                if(isdigit(*it)) {
                                    substring = substr(input, it);
                                    val = stoi(substring, &p);
                                    it += p;
                                    while(*it == ' ' && it < input.end())
                                        ++it;
                                    if(*it == ')')
                                        break;
                                    else
                                        throw ExplanatoryException("Expected ')'", input, it);
                                }
                                else
                                    throw ExplanatoryException("Expected argument's name or number of it", input, it);
                            }
                            while(*it == ' ' && it < input.end())
                                ++it;
                            if(*it == ',') {
                                ++it;
                                while(*it == ' ' && it < input.end())
                                    ++it;
                                continue;
                            }
                            if(*it == ')') {
                                val = arguments.size();
                                break;
                            }
                            throw ExplanatoryException("Expected end of arguments list or comma", input, it);
                        }
                        ++it;
                        if(it != input.end())
                            throw ExplanatoryException("Expected The End", input, it);
                        AbstructFunction *f = globalscope.getFunction(name, val);
                        if(f)
                            cout << f->getView() << endl;
                        else
                            cout << "Function not find" << endl;
                    }
                    else
                        throw ExplanatoryException("Unexpected character", input, it);
                }
                else if((startedwith(input, "delete ") && (pos = 7)) || (startedwith(input, "del ") && (pos = 4))) {
                    it = input.begin()+pos;
                    while(*it == ' ' && it < input.end())
                        ++it;
                    name = getName(it, input.end());
                    if(name == "")
                        throw ExplanatoryException("Expected name of function or variable", input, it);
                    while(*it == ' ' && it < input.end())
                        ++it;
                    if(it == input.end()) {
                        if(globalscope.deleteVariable(name))
                            cout << "Variable was deleted" << endl;
                        else
                            cout << "Variable not find" << endl;
                    }
                    else if(*it == '(') {
                        int val = 0;
                        ++it;
                        while(*it == ' ' && it < input.end())
                            ++it;
                        vector<string> arguments;
                        if(*it != ')') while(true) {
                            arguments.push_back(getName(it, input.end()));
                            if(arguments.back() == "") {
                                if(isdigit(*it)) {
                                    substring = substr(input, it);
                                    val = stoi(substring, &p);
                                    it += p;
                                    while(*it == ' ' && it < input.end())
                                        ++it;
                                    if(*it == ')')
                                        break;
                                    else
                                        throw ExplanatoryException("Expected ')'", input, it);
                                }
                                else
                                    throw ExplanatoryException("Expected argument's name or number of it", input, it);
                            }
                            while(*it == ' ' && it < input.end())
                                ++it;
                            if(*it == ',') {
                                ++it;
                                while(*it == ' ' && it < input.end())
                                    ++it;
                                continue;
                            }
                            if(*it == ')') {
                                val = arguments.size();
                                break;
                            }
                            throw ExplanatoryException("Expected end of arguments list or comma", input, it);
                        }
                        ++it;
                        if(it != input.end())
                            throw ExplanatoryException("Expected The End", input, it);
                        if(globalscope.deleteFunction(name, val))
                            cout << "Function was deleted" << endl;
                        else
                            cout << "Function not find" << endl;
                    }
                    else
                        throw ExplanatoryException("Unexpected character", input, it);
                }
                else {
                    Function *f = new Function(&globalscope);
                    f->setName("this");
                    f->setArgsBody({}, input);
                    cout << "= " << f->calculate({}) << endl;
                }
            }
            catch(ExplanatoryException const &e) {
                cerr << "-------------------------------------------\n";
                cerr << "Error: " << e.whole() << endl;
                cerr << "-------------------------------------------\n";
            }
            catch(Exception const &e) {
                cerr << "Error: " << e.what() << endl;
            }
            catch(...) {
                cerr << "What is it" << endl;
            }
        } // While

    return 0;
}
