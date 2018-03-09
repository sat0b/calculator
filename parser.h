#pragma once
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
    std::map<std::string, int> global_var;
    std::stack<std::map<std::string, int>> local_var;
    std::map<std::string, size_t> functions;
    bool break_flg = false;
    void call_function(std::string name, std::vector<int> args);
    void eval_expression(int priority);
    void eval_factor();
    int read_cond();
    void read_stat();
    void read_symbol_stat();
    void read_return_stat();
    void read_function_call(std::string name);
    void read_block();
    void read_print_stat();
    void read_numeric_stat();
    void read_for_stat();
    void read_if_stat();
    void read_function_def();
    bool skip(TokenKind kind);
    void parse_error(std::string msg);
    Token consume();

  public:
    Parser(std::unique_ptr<Lexer> lexer);
    void run();
};
