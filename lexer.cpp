#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string &code) : code_(code) {
    // Lexical analysis
    p = 0;
    c = code_[p];
    for (;;) {
        Token token = lex();
        tokens.push_back(token);
        if (token.get_kind() == CodeEnd)
            break;
    }
    // Initalize token position tkp
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

bool Lexer::skip_whitespace() {
    char c = read();
    if (c == ' ') {
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

Token Lexer::lex() {
    char c = read();
    if (c == eof)
        return Token::get_code_end_token();
    if (skip_whitespace())
        return lex();
    if (isdigit(c))
        return read_num();
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

bool Lexer::jump_if() {
    for (size_t pc = tkp; pc < tokens.size(); pc++) {
        if (tokens[pc].get_kind() == ElseIf || tokens[pc].get_kind() == Else ||
            tokens[pc].get_kind() == End) {
            tkp = pc;
            return true;
        }
    }
    return false;
}

bool Lexer::jump_back(TokenKind token_kind) {
    for (size_t pc = tkp; pc > 0; pc--) {
        if (tokens[pc].get_kind() == token_kind) {
            tkp = pc + 1;
            return true;
        }
    }
    return false;
}

bool Lexer::jump_block() {
    int count = 0;
    for (size_t pc = tkp; pc < tokens.size(); pc++) {
        TokenKind kind = tokens[pc].get_kind();
        if (count == 0) {
            if (kind == End || kind == ElseIf || kind == Else) {
                tkp = pc;
                return true;
            }
        }
        if (kind == For || kind == If)
            count++;
        else if (kind == End)
            count--;
    }
    return false;
}

void Lexer::skip() { tkp++; }

bool Lexer::skip_until(TokenKind token_kind) {
    for (Token token = next_token(); token.get_kind() != CodeEnd;
         token = next_token()) {
        if (token.get_kind() == token_kind)
            return true;
    }
    return false;
}

bool Lexer::match(TokenKind token_kind) {
    Token token = read_token();
    if (token.get_kind() == token_kind)
        return true;
    return false;
}
