#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
  std::string code;
  int p;
  std::vector<Token> tokens;
  const std::vector<char> operators{'+', '-', '*', '/', '(', ')', '=',
                                    '&', '|', ';', '!', '%', '<', '>'};
  const std::vector<std::string> twoLenthOperators{
      "==", "!=", "<=", ">=", "&&", "||"};
  bool checkOperator(char c) const;

public:
  void init(const std::string &code);
  Token nextToken();
  void showTokens() const;
  bool skip(TokenKind tokenKind);
};
