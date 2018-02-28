#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
  private:
    std::string code_;
    int p;
    int tkp;
    char c;
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
    void show_tokens() const;
    bool skip(TokenKind token_kind);
    void skip();
    bool match(TokenKind token_kind);
};
