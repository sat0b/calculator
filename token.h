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
    Comma,
    While,
    If,
    ElseIf,
    Else,
    End,
    Break,
    Return,
    Function,
    FunctionDef,
    String,
    Block,
    Expr,
    CodeEnd,
};

class Token {
  private:
    TokenKind kind;
    std::string token;
    TokenKind get_token_kind(const std::string &token) const;

  public:
    Token();
    explicit Token(std::string str);
    explicit Token(TokenKind kind);
    Token(std::string str, TokenKind token_kind);
    TokenKind get_kind() const;
    int get_value() const;
    std::string get_name() const;
    static Token get_code_end_token();
};
