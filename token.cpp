#include "token.h"
#include <iostream>

TokenKind Token::getTokenKind(const std::string &token) const {
  if (all_of(token.cbegin(), token.cend(), isdigit))
    return Integer;
  if (token == "+")
    return Plus;
  if (token == "-")
    return Minus;
  if (token == "*")
    return Product;
  if (token == "/")
    return Divide;
  if (token == "(")
    return LeftBracket;
  if (token == ")")
    return RightBracket;
  if (token == "=")
    return Assign;
  if (token == "&&")
    return And;
  if (token == "||")
    return Or;
  if (token == "==")
    return Equal;
  if (token == "!=")
    return NotEqual;
  if (token == "<")
    return LessThan;
  if (token == "<=")
    return LessEqual;
  if (token == ">")
    return GreaterThan;
  if (token == ">=")
    return GreaterEqual;
  if (token == "%")
    return Mod;
  if (token == ";")
    return StatementEnd;
  if (token == "print")
    return Print;
  if (token == "for")
    return For;
  if (token == "while")
    return While;
  if (token == "if")
    return If;
  if (token == "elseif")
    return ElseIf;
  if (token == "else")
    return Else;
  if (token == "break")
    return Break;
  if (token == "return")
    return Return;
  if (token == "function")
    return Function;
  if (token == "end")
    return End;
  return Variable;
}

Token::Token(std::string str, TokenKind tokenKind)
    : token(str), kind(tokenKind) {}

Token::Token(std::string str) {
  this->kind = getTokenKind(str);
  this->token = str;
}

Token::Token(TokenKind kind) {
  this->kind = kind;
  this->token = "";
}

TokenKind Token::getKind() const { return kind; }

int Token::getValue() const {
  if (std::all_of(token.cbegin(), token.cend(), isdigit)) {
    // TODO: integer range check
    return std::stoi(token);
  }
  return 0;
}

std::string Token::getName() const { return this->token; }

Token Token::getCodeEndToken() {
  Token token(CodeEnd);
  return token;
}
