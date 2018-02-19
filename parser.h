#pragma once

#include "token.h"
#include <map>
#include <stack>

enum class ErrorType { SyntaxError, NameError, SymbolError };

class Error {
private:
  bool err = false;
  ErrorType errType;
  std::string errMessage;

public:
  bool state();
  void printErrorMessage();
  void setSyntaxError(std::string msg);
  void setNameError(std::string msg);
  void setSymbolError(std::string msg);
};

class Parser {
private:
  Tokenizer tokenizer;
  Token token;
  std::stack<int> stack;
  std::map<std::string, int> variables;
  Error error;
  bool replMode;

  void next();
  void checkKind(const TokenKind kind);
  void statement();
  void orExpression();
  void andExpression();
  void equalExpression();
  void thanExpression();
  void expression();
  void term();
  void factor();
  void operate(const TokenKind op);
  void showVariableTable() const;

public:
  void run(const std::string &line, bool replMode);
};
