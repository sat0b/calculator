#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string &code) : code_(code) {
  p = 0;
  c = code_[p];
}

char Lexer::read() {
  if (p < code_.length())
    return code_[p];
  return eof;
}

char Lexer::consume() {
  if (p < code_.length())
    return code_[p++];
  return eof;
}

bool Lexer::skipWhitespace() {
  char c = read();
  if (c == ' ') {
    consume();
    return true;
  }
  return false;
}

Token Lexer::readNum() {
  std::string str;
  while (isdigit(code_[p]))
    str += consume();
  return Token(str, Integer);
}

Token Lexer::readOperator() {
  char c = consume();
  char cn = read();
  if (cn != eof) {
    std::string op;
    op += c;
    op += cn;
    if (op == "==" || op == "!=" || op == "<=" || op == ">=" || op == "&&" ||
        op == "||") {
      consume();
      return Token(op);
    }
  }
  return Token(std::string(1, c));
}

Token Lexer::readIdentifier() {
  std::string str;
  while (isalpha(read()) || read() == '_')
    str += consume();
  return Token(str);
}

Token Lexer::nextToken() {
  char c = read();
  if (c == eof)
    return Token::getCodeEndToken();
  if (skipWhitespace())
    return nextToken();
  if (isdigit(c))
    return readNum();
  else if (isalpha(c))
    return readIdentifier();
  else
    return readOperator();
}

bool Lexer::skip(TokenKind tokenKind) {
  Token token = nextToken();
  if (token.getKind() == tokenKind)
    return false;
  return true;
}
