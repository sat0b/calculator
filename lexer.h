#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
  std::string code_;
  int p;
  char c;
  const char eof = -1;
  std::vector<Token> tokens;
  const std::vector<char> operators{'+', '-', '*', '/', '(', ')', '=',
                                    '&', '|', ';', '!', '%', '<', '>'};
  const std::vector<std::string> twoLenthOperators{
      "==", "!=", "<=", ">=", "&&", "||"};
  char read();
  char consume();
  bool checkOperator(char c) const;
  bool skipWhitespace();
  Token readNum();
  Token readOperator();
  Token readIdentifier();
  Token readKeyword();
  Token readEnd();

public:
  Lexer(const std::string &code);
  Token nextToken();
  void showTokens() const;
  bool skip(TokenKind tokenKind);
};
