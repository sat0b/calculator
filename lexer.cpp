#include "lexer.h"
#include <iostream>

bool Lexer::checkOperator(char c) const {
  return std::any_of(operators.cbegin(), operators.cend(), [=](int x) {
    if (x == c)
      return true;
    else
      return false;
  });
}

Lexer::Lexer(const std::string &code) : code_(code) {
  p = 0;
  c = code_[p];
  init();
}

bool Lexer::skipWhitespace() {
  if (code_[p] == ' ') {
    p++;
    return true;
  }
  return false;
}

Token Lexer::readNum() {
  std::string str;
  while (isdigit(code_[p])) {
    str += code_[p++];
  }
  Token token(str, Integer);
  return token;
}

Token Lexer::readOperator() {
  std::string str;
  if (p + 1 < code_.length()) {
    std::string subcode = code_.substr(p, 2);
    auto itr = std::find(twoLenthOperators.cbegin(), twoLenthOperators.cend(),
                         subcode);
    if (itr != twoLenthOperators.cend()) {
      // two length
      str = subcode;
      p += 2;
    } else {
      // single length
      str = code_[p++];
    }
  } else {
    str = code_[p++];
  }
  Token token(str);
  return token;
}

Token Lexer::readIdentifier() {
  std::string str;
  while (code_[p] != ' ' && !checkOperator(code_[p])) {
    str += code_[p++];
  }
  Token token(str);
  return token;
}

void Lexer::init() {}

Token Lexer::nextToken() {
  if (p < code_.length()) {
    if (skipWhitespace())
      return nextToken();
    if (isdigit(code_[p]))
      return readNum();
    else if (checkOperator(code_[p]))
      return readOperator();
    else
      return readIdentifier();
  }
  return Token::getCodeEndToken();
}

bool Lexer::skip(TokenKind tokenKind) {
  Token token = nextToken();
  if (token.getKind() == tokenKind)
    return false;
  return true;
}
