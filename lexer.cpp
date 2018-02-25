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

void Lexer::init(const std::string &code) {
  tokens.clear();
  int i = 0;
  while (i < code.length()) {
    if (code[i] == ' ') {
      i++;
      continue;
    }
    std::string str;
    // digit
    if (isdigit(code[i])) {
      while (isdigit(code[i])) {
        str += code[i++];
      }
    }
    // operator
    else if (checkOperator(code[i])) {
      if (i + 1 < code.length()) {
        std::string subcode = code.substr(i, 2);
        auto itr = std::find(twoLenthOperators.cbegin(),
                             twoLenthOperators.cend(), subcode);
        if (itr != twoLenthOperators.cend()) {
          // two length
          str = subcode;
          i += 2;
        } else {
          // single length
          str = code[i++];
        }
      } else {
        str = code[i++];
      }
    }
    // variable
    else {
      while (code[i] != ' ' && !checkOperator(code[i])) {
        str += code[i++];
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
