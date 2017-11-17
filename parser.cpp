#include "parser.hpp"

#include <iostream>

Cell createAtom (const Token& tok) {
    switch (tok.type) {
    case STRING:
        return Cell(String, tok.data);
        break;
    case NUMERIC:
        return Cell(Number, tok.data);
        break;
    case KEY_WORD:
        return Cell(Symbol, tok.data);
        break;
    default:
        std::cout << "Error here!!" << tok.type << '\n';
        exit(20);
    }
}


Cell parse(TokenListCIter& tli, const TokenListCIter& end) {
    if (tli->type == OPENING_PARREN) {
        Cell c(List);
        tli++;
        while (tli->type != CLOSING_PARREN) {
            c.list.push_back(parse(tli, end));
            if (++tli >= end) exit(30); // Missmatching parrens.... Sigh...
        }
        return c;
    } else {
        return createAtom(*tli);
    }
}
