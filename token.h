#pragma once

#include <string>
#include <vector>

enum TokenKind {
  Integer,
  Product,
  Plus,
  Divide,
  Minus,
  Symbol,
  LeftBracket,
  RightBracket,
  Variable,
  Assign,
  Print,
  And,
  Or,
  Equal,
  NotEqual,
  LessThan,
  LessEqual,
  GreaterThan,
  GreaterEqual,
  Mod,
  StatementEnd,
  End,
};

std::string getTokenString(TokenKind kind);

class Token {
private:
  TokenKind kind;
  std::string token;
  TokenKind getTokenKind(const std::string &token) const;

public:
  Token(){};
  Token(std::string str);
  Token(TokenKind kind);
  TokenKind getKind() const;
  int getValue() const;
  std::string getName() const;
  std::string to_string();
  static Token getEndToken();
};

class Tokenizer {
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
};
