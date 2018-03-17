#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string &code) : code_(code) {
    // Lexical analysis
    p = 0;
    for (;;) {
        Token token = lex();
        tokens.push_back(token);
        if (token.get_kind() == CodeEnd)
            break;
    }
    // Initialize token position tkp
    tkp = 0;
}

char Lexer::read() {
    if (p < code_.length())
        return code_[p];
    return eof;
}

char Lexer::consume() {
    if (p < code_.length())
        return code_[p++];
    return eof;
}

bool Lexer::skip_mark() {
    char c = read();
    if (c == ' ' || c == ';') {
        consume();
        return true;
    }
    return false;
}

Token Lexer::read_num() {
    std::string str;
    while (isdigit(code_[p]))
        str += consume();
    return Token(str, Integer);
}

Token Lexer::read_operator() {
    char c = consume();
    char cn = read();
    if (cn != eof) {
        std::string op;
        op += c;
        op += cn;
        if (op == "==" || op == "!=" || op == "<=" || op == ">=" ||
            op == "&&" || op == "||") {
            consume();
            return Token(op);
        }
    }
    return Token(std::string(1, c));
}

Token Lexer::read_identifier() {
    std::string str;
    while (isalpha(read()) || read() == '_')
        str += consume();
    return Token(str);
}

Token Lexer::read_str() {
    std::string str;
    consume(); // begin "
    while (read() != '"')
        str += consume();
    consume(); // end "
    return Token(str, String);
}

Token Lexer::lex() {
    char c = read();
    if (c == eof)
        return Token::get_code_end_token();
    if (skip_mark())
        return lex();
    if (isdigit(c))
        return read_num();
    else if (c == '"')
        return read_str();
    else if (isalpha(c))
        return read_identifier();
    else
        return read_operator();
}

Token Lexer::next_token() { return tokens[tkp++]; }

Token Lexer::read_token() { return tokens[tkp]; }

bool Lexer::skip(TokenKind token_kind) {
    Token token = read_token();
    if (token.get_kind() != token_kind)
        return false;
    tkp++;
    return true;
}

void Lexer::expect_skip(TokenKind token_kind) {
    Token token = next_token();
    if (token.get_kind() != token_kind) {
        Token expect(token_kind);
        std::cerr << "Syntax error, expected " << expect.get_name()
                  << ", but got " << token.get_name() << std::endl;
        std::exit(1);
    }
}

bool Lexer::match(TokenKind token_kind) {
    Token token = read_token();
    if (token.get_kind() == token_kind)
        return true;
    return false;
}
