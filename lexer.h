#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
  private:
    std::string code_;
    size_t p;
    size_t tkp;
    const char eof = -1;
    std::vector<Token> tokens;
    char read();
    char consume();
    bool skip_mark();
    Token read_num();
    Token read_operator();
    Token read_identifier();
    Token read_str();
    Token lex();

  public:
    explicit Lexer(const std::string &code);
    Token next_token();
    Token read_token();
    Token read_token(int offset);
    bool skip(TokenKind token_kind);
    void expect_skip(TokenKind token_kind);
    bool match(TokenKind token_kind);
    bool match(TokenKind token_kind, int offset);
};
