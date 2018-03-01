#pragma once
#include "lexer.h"
#include "stack.h"
#include "token.h"
#include <map>

enum class ErrorType { SyntaxError, NameError, SymbolError };

class Error {
  private:
    bool err = false;
    ErrorType err_type;
    std::string err_message;

  public:
    bool state();
    void reset();
    void print_error_message();
    void set_syntax_error(std::string msg);
    void set_name_error(std::string msg);
    void set_symbol_error(std::string msg);
};

class Parser {
  private:
    Lexer *lexer;
    Stack stack;
    std::map<std::string, int> variables;
    Error error;

    void statement();
    void expression(int priority);
    void or_expression();
    void and_expression();
    void equal_expression();
    void than_expression();
    void term();
    void factor();
    int operate(const TokenKind op);
    void show_variable_table() const;
    void variable_statement();
    void print_statement();
    void numeric_statement();
    void block();
    void for_statement();
    void if_statement();
    void else_if_statement();
    void else_statement();
    bool skip(TokenKind kind);
    void parse_error();
    void parse_error(std::string msg);
    Token consume();

  public:
    Parser(Lexer *lexer);
    void run();
};
