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
    Token token;
    Stack stack;
    std::map<std::string, int> variables;
    Error error;
    bool repl_mode;

    void next();
    void check_kind(const TokenKind kind);
    void statement();
    void or_expression();
    void and_expression();
    void equal_expression();
    void than_expression();
    void expression();
    void term();
    void factor();
    void operate(const TokenKind op);
    void show_variable_table() const;
    void variable_statement();
    void print_statement();
    void numeric_statement();
    void block();
    void for_statement();
    void if_statement();
    void else_if_statement();
    void else_statement();
    void skip_until(TokenKind kind);
    Token consume();
    bool skip(TokenKind kind);

  public:
    Parser(Lexer *lexer);
    void run(const std::string &line, bool repl_mode);
    void run();
};
