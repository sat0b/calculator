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
    // void call_function(std::string name, std::vector<int> args);
    // void eval_expression(int priority);
    // void eval_factor();
    // int read_cond();
    Ast *read_stat();
    Ast *read_symbol_stat();
    Ast *read_expr(int);
    Ast *read_factor();
    Ast *read_block();
    Ast *read_for();
    Ast *read_if();
    Ast *read_function_def();
    Ast *read_return();
    // void read_return_stat();
    // void read_function_call(std::string name);
    // void read_block();
    Ast *read_print_stat();
    // void read_numeric_stat();
    // void read_for_stat();
    // void read_if_stat();
    // void read_function_def();
    void parse_error(std::string msg);
    Token consume();

  public:
    Parser(std::unique_ptr<Lexer> lexer);
    std::vector<Ast *> parse();
};
