#ifndef LEXER_H_
#define LEXER_H_

#include <vector>
#include <algorithm>
#include <sstream>


struct Token;
using Tokenlist = std::vector<Token>;
using csi = std::string::const_iterator;
using TokenList = std::vector<Token>;
using TokenListCIter = TokenList::const_iterator;

enum TokenType {
    OPENING_PARREN,
    CLOSING_PARREN,
    KEY_WORD,
    NUMERIC,
    STRING
};

struct Token {
    TokenType type;
    std::string data;
    int index;

    Token(TokenType type, std::string data, int index):
        type(type), data(data), index(index) {}
};

void skippWhiteSpace(csi& si, const csi& end);
std::string strFromPtr(csi begin, size_t length);

class Tokenizer {
private:
    const std::string source;

public:
    Tokenizer(const std::string& s): source(s) {}
    TokenList tokenize() const;

private:
    Token consumeString (csi& si) const;
    static bool isNumeric(char c) {
        if (c - '0' <= 9 && c - '0' >= 0) {
            return true;
        }
        return false;
    }

    static bool isWordy(char c) {
        return (c != ' ' && c != '(' && c != ')' && c != '\n');
    }

    template<typename F>
    Token consumeToToken(csi& si, TokenType t, F cond) const {
        std::ostringstream oss;
        while(si < source.cend() && cond(*si)) {
            oss << *si;
            si++;
        }
        return Token(t, oss.str(), si-source.cbegin());
    }
};

#endif
