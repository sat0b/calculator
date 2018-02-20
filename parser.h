#pragma once

#include "stack.h"
#include "token.h"
#include <map>

enum class ErrorType { SyntaxError, NameError, SymbolError };

class Error {
private:
  bool err = false;
  ErrorType errType;
  std::string errMessage;

public:
  bool state();
  void reset();
  void printErrorMessage();
  void setSyntaxError(std::string msg);
  void setNameError(std::string msg);
  void setSymbolError(std::string msg);
};

class Parser {
private:
  Tokenizer tokenizer;
  Token token;
  Stack stack;
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
