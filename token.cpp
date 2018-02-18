#include "token.h"
#include <iostream>
#include <vector>

std::string getTokenString(TokenKind kind) {
  switch (kind) {
  case Integer:
    return "[Integer]";
  case Product:
    return "[Product]";
  case Plus:
    return "[Plus]";
  case Divide:
    return "[Divide]";
  case Minus:
    return "[Minus]";
  case Symbol:
    return "[Symbol]";
  case LeftBracket:
    return "[LeftBracket]";
  case RightBracket:
    return "[RightBracket]";
  case Variable:
    return "[Variable]";
  case Assign:
    return "[Assign]";
  case And:
    return "[And]";
  case Or:
    return "[Or]";
  case Equal:
    return "[Equal]";
  case NotEqual:
    return "[NotEqual]";
  case LessThan:
    return "[LessThan]";
  case LessEqual:
    return "[LessEqual]";
  case GreaterThan:
    return "[GreaterThan]";
  case GreaterEqual:
    return "[GreaterEqual]";
  case Mod:
    return "[Mod]";
  case Print:
    return "[Print]";
  case StatementEnd:
    return "[StatementEnd]";
  case End:
    return "[End]";
  }
}

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
  return Variable;
}

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
    return std::stoi(token);
  }
  return 0;
}

std::string Token::getName() const { return this->token; }

std::string Token::to_string() {
  std::string ret = getTokenString(kind);
  return ret + " " + token;
}

Token Token::getEndToken() {
  Token token(End);
  return token;
}

bool Tokenizer::checkOperator(char c) const {
  return std::any_of(operators.cbegin(), operators.cend(), [=](int x) {
    if (x == c)
      return true;
    else
      return false;
  });
}

void Tokenizer::init(const std::string &code) {
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
        str += code[i];
        i++;
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
          str = code[i];
          i++;
        }
      } else {
        str = code[i];
        i++;
      }
    }
    // variable
    else {
      while (code[i] != ' ' && !checkOperator(code[i])) {
        str += code[i];
        i++;
      }
    }
    Token token(str);
    tokens.push_back(token);
  }
  p = 0;
}

Token Tokenizer::nextToken() {
  if (p < tokens.size())
    return tokens[p++];
  return Token::getEndToken();
}

void Tokenizer::showTokens() const {
  for (auto t : tokens) {
    std::cout << t.to_string() << std::endl;
  }
}
