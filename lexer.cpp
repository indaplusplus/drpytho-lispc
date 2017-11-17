#include "lexer.hpp"

void skippWhiteSpace(csi& si, const csi& end) {
    while(si < end && (*si) == ' ') {
        si++;
    }
}

std::string strFromPtr(csi begin, size_t length) {
    std::ostringstream oss;

    for (auto si = begin; si < begin + length; si++) {
        oss << *si;
    }
    return oss.str();
}


Token Tokenizer::consumeString (csi& si) const {
    si++;
    auto tok = consumeToToken(si, TokenType::STRING, [](char c){
            return c != '"';
        });
    si++;
    if (si == source.end()) {
        exit(-12); // String not ended.
    }
    return tok;
}

TokenList Tokenizer::tokenize() const {
    TokenList tl;

    auto si = source.cbegin();
    const auto begin = source.cbegin();
    const auto end = source.cend();

    while (si < end) {
        if (*si == ' ' || *si == '\n') {
            si++;
            continue;
        }

        if (*si == '(') {
            tl.push_back(Token(TokenType::OPENING_PARREN,
                               "(",
                               static_cast<int>(si - begin)));
            si++;
            continue;
        }

        if (*si == ')') {
            tl.push_back(Token(TokenType::CLOSING_PARREN,
                               ")",
                               static_cast<int>(si - begin)));
            si++;
            continue;
        }

        if (*si == '"') {
            tl.push_back(consumeString(si));
            continue;
        }

        if(isNumeric(*si)) {
            tl.push_back(consumeToToken(si, TokenType::NUMERIC, isNumeric));
            continue;
        }

        if(isWordy(*si)) {
            tl.push_back(consumeToToken(si, TokenType::KEY_WORD, isWordy));
            continue;
        }

        si++;
    }
    return tl;
}
