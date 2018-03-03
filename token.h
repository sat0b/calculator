#pragma once

#include <string>
#include <vector>

enum TokenKind {
    Symbol,
    Integer,
    Product,
    Plus,
    Divide,
    Minus,
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
    For,
    While,
    If,
    ElseIf,
    Else,
    End,
    Break,
    Return,
    Function,
    StatementEnd,
    CodeEnd,
};

class Token {
  private:
    TokenKind kind;
    std::string token;
    TokenKind get_token_kind(const std::string &token) const;

  public:
    Token();
    Token(std::string str);
    Token(std::string str, TokenKind token_kind);
    Token(TokenKind kind);
    TokenKind get_kind() const;
    int get_value() const;
    std::string get_name() const;
    std::string to_string();
    static Token get_code_end_token();
};
