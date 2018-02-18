#pragma once

#include "token.h"
#include <map>
#include <stack>

class Parser {
private:
  Tokenizer tokenizer;
  Token token;
  std::stack<int> stack;
  std::map<std::string, int> variables;
  bool err;

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
  void run(const std::string &line);
};
