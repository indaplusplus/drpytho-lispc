#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <sstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "runner.hpp"

void print_cell(const Cell& cell) {
    std::cout << "Cell: " << cell.type << " - " << cell.val << '\n';
}

void print_cells(const Cell& cell) {
    print_cell(cell);
    for (const auto& c : cell.list) {
        print_cells(c);
    }
}

int main() {
    const std::string s(R"(
(def (trollo lollo) '(fn (a b) (+ a b)))
(list 1 2 (list 3 "Hello this is nice"))
)");

    const std::string code(R"(
(prog
  (def x true)
  (def i 0)
  (def not (fn (x) (if x false true)))
  (loop (not (= i 20)) (prog
    (print! i)
    (set! i (+ i 1))
    (if (= i 15) (set! x false) ()))))
)");

    const std::string code2(R"(
(prog
  (def fib (fn (x) (prog
    (if (= x 1) 1
        (if (= x 2) 1
          (+ (fib (- x 1)) (fib (- x 2))))))))
  (print! (fib 25)))
)");

    const std::string code3(R"(
(prog
  (print! (let ((x 5)
        (y (+ x 3))
        (z "Nine"))
       (prog
         (print! (+ x y))
         (print! z)
         z)))
  (print! 1))
)");

    Tokenizer t(code3);
    auto tokens = t.tokenize();

    /*
    for (const auto& token : tokens) {
        std::cout << token.type << " " <<  token.data << ' ';
    }
    */
    auto iter = tokens.cbegin();
    auto cell = parse(iter, tokens.cend());

    //    print_cells(cell);

    Environment e(nullptr);

    addGlobalVars(e);

    auto c = eval(cell, &e);

    //std::cout << c.val << '\n';

    /*
    for (const auto& token : tokens) {
        std::cout <<  token.data << ' ';
        }*/
    return 0;
}
