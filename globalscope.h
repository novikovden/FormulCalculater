#ifndef GLOBAL_SCOPE
#define GLOBAL_SCOPE

#include <string>
#include <vector>

class AbstructFunction;
class Variable;

class Scope {
public:
    Scope();
    virtual ~Scope();

    virtual AbstructFunction* getFunction(std::string const &name, int carg) const = 0;
    virtual Variable* getVariable(std::string const &name) const = 0;
};

class GlobalScope: public Scope {
public:
    GlobalScope();
    virtual ~GlobalScope();

    GlobalScope& defineFunction(AbstructFunction *newfun, bool *redefined = nullptr);
    GlobalScope& defineVariable(Variable *newvar, bool *redefined = nullptr);

    bool deleteFunction(AbstructFunction const *fun);
    bool deleteFunction(std::string const &name, int carg);
    bool deleteVariable(Variable const *variable);
    bool deleteVariable(std::string const &name);

    virtual AbstructFunction* getFunction(std::string const &name, int carg) const override;
    virtual Variable* getVariable(std::string const &name) const override;

    std::string getViewFunction(std::string const &name, int carg) const;
    std::string getViewVariable(std::string const &name) const;

    std::vector<std::string> getNamesFunctions() const;
    std::vector<std::string> getNamesVariables() const;

private:
    void _sortFunctions();
    void _sortVariables();

    std::vector<AbstructFunction*> _functions;
    std::vector<Variable*> _variables;
};

#endif // GLOBAL_SCOPE
