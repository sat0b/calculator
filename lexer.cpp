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
  int p = 0;
  c = code_[p];
  init();
}

void Lexer::init() {
  tokens.clear();
  int i = 0;
  while (i < code_.length()) {
    if (code_[i] == ' ') {
      i++;
      continue;
    }
    std::string str;
    // digit
    if (isdigit(code_[i])) {
      while (isdigit(code_[i])) {
        str += code_[i++];
      }
    }
    // operator
    else if (checkOperator(code_[i])) {
      if (i + 1 < code_.length()) {
        std::string subcode = code_.substr(i, 2);
        auto itr = std::find(twoLenthOperators.cbegin(),
                             twoLenthOperators.cend(), subcode);
        if (itr != twoLenthOperators.cend()) {
          // two length
          str = subcode;
          i += 2;
        } else {
          // single length
          str = code_[i++];
        }
      } else {
        str = code_[i++];
      }
    }
    // variable
    else {
      while (code_[i] != ' ' && !checkOperator(code_[i])) {
        str += code_[i++];
      }
    }
    Token token(str);
    tokens.push_back(token);
  }
  p = 0;
}

Token Lexer::nextToken() {
  if (p < tokens.size())
    return tokens[p++];
  return Token::getCodeEndToken();
}

void Lexer::showTokens() const {
  for (auto t : tokens) {
    std::cout << t.toString() << std::endl;
  }
}

bool Lexer::skip(TokenKind tokenKind) {
  Token token = nextToken();
  if (token.getKind() == tokenKind)
    return false;
  return true;
}
