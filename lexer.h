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
    bool skip_whitespace();
    Token read_num();
    Token read_operator();
    Token read_identifier();
    Token read_keyword();
    Token read_end();
    Token lex();

  public:
    Lexer(const std::string &code);
    Token next_token();
    Token read_token();
    bool skip(TokenKind token_kind);
    bool skip_until(TokenKind token_kind);
    void skip();
    bool match(TokenKind token_kind);
    bool jump_if();
    bool jump_back(TokenKind token_kind);
    bool jump_block();
};
