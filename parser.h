#pragma once
#include "lexer.h"
#include "stack.h"
#include "token.h"
#include <map>

class Parser {
  private:
    Lexer *lexer;
    Stack stack;
    std::map<std::string, int> variables;

    void statement();
    void expression(int priority);
    void factor();
    int operate(const TokenKind op);
    void variable_statement();
    void print_statement();
    void numeric_statement();
    void block();
    void for_statement();
    void if_statement();
    int parse_truth();
    bool skip(TokenKind kind);
    void parse_error(std::string msg);
    Token consume();

  public:
    Parser(Lexer *lexer);
    void run();
};
