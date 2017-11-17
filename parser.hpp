#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <cctype>
#include <map>

#include "lexer.hpp"

struct Cell;
struct Token;
struct Environment;

using Cells = std::vector<Cell>;
using Tokenlist = std::vector<Token>;
using csi = std::string::const_iterator;
using coord = std::tuple<int, int>;
using proc_t = std::function<Cell(const Cells&)>;

enum CellType {
    Symbol,
    Number,
    String,
    List,
    Proc,
    Fn
};


struct Cell {
    CellType type;
    std::string val;
    Cells list;
    proc_t proc;
    Environment* env;

    Cell(CellType t = Symbol): type(t), val(""), list(), env(nullptr){}
    Cell(CellType t, const std::string& val): type(t), val(val), list(), env(nullptr) {}
    Cell(proc_t proc): type(Proc), val(""), list(), proc(proc), env(nullptr) {}
};

Cell parse(TokenListCIter& tli, const TokenListCIter& end);

#endif // PARSER_HPP
