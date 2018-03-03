#pragma once
#include "lexer.h"
#include "stack.h"
#include "token.h"
#include <map>
#include <memory>

class Parser {
  private:
    std::unique_ptr<Lexer> lexer;
    Stack stack;
    std::map<std::string, int> variables;

    void eval_expression(int priority);
    void eval_factor();
    int read_cond();
    void read_stat();
    void read_block();
    void read_print_stat();
    void read_variable_stat();
    void read_numeric_stat();
    void read_for_stat();
    void read_if_stat();
    bool skip(TokenKind kind);
    void parse_error(std::string msg);
    Token consume();

  public:
    Parser(std::unique_ptr<Lexer> lexer);
    void run();
};
