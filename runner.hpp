#ifndef RUNNER_HPP
#define RUNNER_HPP

#include<map>
#include<string>
#include<sstream>
#include "parser.hpp"

using Value = Cell;
using Key = std::string;
using Refrences = std::map<Key, Value>;

struct Environment {
private:
    Refrences refs;
    Environment* env;

public:
    explicit Environment(Environment* env): refs(), env(env) {}

    template<typename IterableCont>
    Environment(IterableCont keys, IterableCont vals, Environment* env): refs(), env(env) {
        auto valIter = vals.cbegin();
        auto keyIter = keys.cbegin();
        while(valIter != vals.cend() && keyIter != keys.cend()) {
            refs.insert_or_assign(keyIter->val, *valIter);
            keyIter++;
            valIter++;
        }
    }

    void setValue(const Key&, Value);
    //void setValue(const Key&, Value&&);
    bool keyExists(const Key&) const;
    Value& getValue(const Key&);
    Value& operator[] (const Key&);
};

const Cell false_sym(Symbol, "false");
const Cell true_sym(Symbol, "true");
const Cell nil(Symbol, "nil");

inline std::string str(long n) { std::ostringstream os; os << n; return os.str(); }

Cell print_sym(const Cells& cells);
Cell add_sym(const Cells& cells);
Cell eq_sym(const Cells& cells);
Cell lt_sym(const Cells& cells);
Cell sub_sym(const Cells& cells);

inline void addGlobalVars(Environment& env) {
    // Global consants
    env.setValue("nil", nil);
    env.setValue("false", false_sym);
    env.setValue("true", true_sym);

    // Pre defined functions
    env.setValue("print!", Cell(print_sym));

    env.setValue("+", Cell(add_sym));
    env.setValue("-", Cell(sub_sym));

    env.setValue("=", Cell(eq_sym));
    env.setValue("<", Cell(lt_sym));


}


Value eval(Cell c, Environment* env);

#endif // RUNNER_HPP
