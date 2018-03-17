#pragma once
#include "ast.h"
#include "lexer.h"
#include "stack.h"
#include "token.h"
#include <map>
#include <memory>
#include <stack>

class Parser {
  private:
    std::vector<std::string> builtins = {"max", "min", "sum"};
    std::unique_ptr<Lexer> lexer;
    Stack stack;
    std::stack<std::map<std::string, int>> local_var;
    std::map<std::string, int> global_var;
    std::map<std::string, size_t> functions;
    bool break_flg = false;
    Ast *read_stat();
    Ast *read_symbol_stat();
    Ast *read_expr(int);
    Ast *read_factor();
    Ast *read_block();
    Ast *read_for();
    Ast *read_if();
    Ast *read_function_def();
    Ast *read_return();
    Ast *read_print_stat();
    void parse_error(std::string msg);

  public:
    explicit Parser(std::unique_ptr<Lexer> lexer);
    std::vector<Ast *> parse();
};
