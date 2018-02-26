#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
  std::string code_;
  int p;
  int tkp;
  char c;
  const char eof = -1;
  std::vector<Token> tokens;
  char read();
  char consume();
  bool skipWhitespace();
  Token readNum();
  Token readOperator();
  Token readIdentifier();
  Token readKeyword();
  Token readEnd();
  Token lex();

public:
  Lexer(const std::string &code);
  Token nextToken();
  void showTokens() const;
  bool skip(TokenKind tokenKind);
};
