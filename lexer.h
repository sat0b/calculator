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
    Token read_keyword();
    Token read_end();
    Token lex();

  public:
    Lexer(const std::string &code);
    Token next_token();
    Token read_token();
    bool skip(TokenKind token_kind);
    bool match(TokenKind token_kind);
    size_t get_addr();
    void jump_addr(size_t addr);
    bool jump_block();
};
