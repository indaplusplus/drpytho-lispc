#include "runner.hpp"

Value& Environment::getValue(const Key& key) {
    if (refs.count(key)) {
        return refs.at(key);
    }
    if (env != nullptr) {
        return env->getValue(key);
    }
    exit(-40);
}

void Environment::setValue(const Key& key, Value value) {
    refs.insert_or_assign(key, value);
}

//void Environment::setValue(const Key& key, Value&& value) {
//    refs.insert_or_assign(key, std::move(value));
//}

Value& Environment::operator[] (const Key& key) {
    return this->getValue(key);
}

bool Environment::keyExists(const Key & key) const {
    return refs.count(key) || (env != nullptr && env->keyExists(key));
}

Cell print_sym(const Cells& cells) {

    for(const auto& cell : cells) {
        std::cout << cell.val;
    }
    std::cout << '\n';
    return nil;
}

Cell add_sym(const Cells& cells) {
    long n = 0;
    for (auto it = cells.cbegin(); it != cells.cend(); it++) {
        if (it->type != Number) {
            return nil;
        }
        n+= std::atol(it->val.c_str());
    }
    return Cell(Number, str(n));
}

Cell eq_sym(const Cells& cells) {
    if (cells[0].val == cells[1].val) {
        return true_sym;
    } else {
        return false_sym;
    }
}

Cell lt_sym(const Cells& cells) {
    if (std::atol(cells[0].val.c_str()) < std::atol(cells[1].val.c_str())) {
        return true_sym;
    } else {
        return false_sym;
    }
}

Cell sub_sym(const Cells& cells) {
    return Cell(Number, str((std::atol(cells[0].val.c_str())
                             - std::atol(cells[1].val.c_str()))));
}

Value eval(Cell c, Environment* env) {
    if (c.type == Symbol) {
        return env->getValue(c.val);
    }

    if (c.type == Number || c.type == String) {
        return c;
    }

    if (c.list.empty()) {
        return nil;
    }

    if (c.list[0].type == Symbol) {
        if (c.list[0].val == "quote") {
            return c.list[1];
        }

        if (c.list[0].val == "if") {
            return eval(eval(c.list[1], env).val == "false" ?
                        (c.list.size() < 4 ? nil : c.list[3]) : c.list[2], env);
        }

        if (c.list[0].val == "set!") {
            env->getValue(c.list[1].val) = eval(c.list[2], env);
            return env->getValue(c.list[1].val);
        }

        if (c.list[0].val == "def") {
            env->setValue(c.list[1].val, eval(c.list[2], env));
            return env->getValue(c.list[1].val);
        }

        if (c.list[0].val == "let") {
            Environment* e = new Environment(env);
            for (const auto& eli : c.list[1].list) {
                if (eli.list[0].type != Symbol) {
                    // Freak
                    exit(45);
                }
                e->setValue(eli.list[0].val, eval(eli.list[1], e));
            }
            auto ret = eval(c.list[2], e);
            delete e;
            return ret;
        }

        if (c.list[0].val == "loop") {
            // (loop (cond) (prog ...))
            Cell last = c.list[1];
            eval(last, env);
            while (eval(c.list[1], env).val != "false") {
                last = eval(c.list[2], env);
            }
            return last;
        }

        if (c.list[0].val == "fn") {
            c.type = Fn;
            c.env = env;
            return c;
        }

        if (c.list[0].val == "prog") {
            for(auto i = c.list.begin() + 1; i+1 != c.list.end(); i++) {
                eval(*i, env);
            }
            return eval(*(c.list.end()-1), env);
        }
    }

    Cell proc(eval(c.list[0], env));

    Cells exps;
    for (auto exp = c.list.begin() + 1; exp != c.list.end(); ++exp)
        exps.push_back(eval(*exp, env));

    if (proc.type == Fn) {
        // Create an environment for the execution of this lambda function
        // where the outer environment is the one that existed* at the time
        // the lambda was defined and the new inner associations are the
        // parameter names with the given arguments.
        // *Although the environmet existed at the time the lambda was defined
        // it wasn't necessarily complete - it may have subsequently had
        // more symbols defined in that environment.
        return eval(/*body*/proc.list[2],
                    new Environment(/*parms*/proc.list[1].list, /*args*/exps, proc.env));
    }
    else if (proc.type == Proc)
        return proc.proc(exps);

    exit(-50);
    return nil;
}
