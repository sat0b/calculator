#include "lexer.h"
#include <iostream>

//  const std::vector<char> operators{'+', '-', '*', '/', '(', ')', '=',
//                                    '&', '|', ';', '!', '%', '<', '>'};
//  const std::vector<std::string> twoLenthOperators{
//      "==", "!=", "<=", ">=", "&&", "||"};
//

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
      str = consume();
    }
  } else {
    str = consume();
  }
  return Token(str);
}

Token Lexer::readIdentifier() {
  std::string str;
  while (read() != ' ' && !checkOperator(read())) {
    str += consume();
  }
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
  else if (checkOperator(c))
    return readOperator();
  else
    return readIdentifier();
}

bool Lexer::skip(TokenKind tokenKind) {
  Token token = nextToken();
  if (token.getKind() == tokenKind)
    return false;
  return true;
}
