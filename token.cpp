#include "token.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

namespace {
std::map<std::string, TokenKind> token_table{
    {"+", Plus},
    {"-", Minus},
    {"*", Product},
    {"/", Divide},
    {"(", LeftBracket},
    {")", RightBracket},
    {"=", Assign},
    {"&&", And},
    {"||", Or},
    {"==", Equal},
    {"!=", NotEqual},
    {"<", LessThan},
    {"<=", LessEqual},
    {">", GreaterThan},
    {">=", GreaterEqual},
    {"%", Mod},
    {"print", Print},
    {"for", For},
    {"while", While},
    {"if", If},
    {"elseif", ElseIf},
    {"else", Else},
    {"break", Break},
    {"return", Return},
    {"function", Function},
    {",", Comma},
    {"end", End},
};
}

TokenKind Token::get_token_kind(const std::string &token) const {
    if (all_of(token.cbegin(), token.cend(), isdigit))
        return Integer;
    if (token_table.count(token) > 0)
        return token_table[token];
    return Symbol;
}

Token::Token() {}

Token::Token(std::string str, TokenKind token_kind)
    : token(str), kind(token_kind) {}

Token::Token(std::string str) {
    this->kind = get_token_kind(str);
    this->token = str;
}

Token::Token(TokenKind kind) {
    this->kind = kind;
    this->token = std::find_if(token_table.begin(), token_table.end(),
                               [=](auto t) { return t.second == kind; })
                      ->first;
}

TokenKind Token::get_kind() const { return kind; }

int Token::get_value() const {
    if (std::all_of(token.cbegin(), token.cend(), isdigit)) {
        // TODO: integer range check
        return std::stoi(token);
    }
    return 0;
}

std::string Token::get_name() const { return this->token; }

Token Token::get_code_end_token() {
    Token token(CodeEnd);
    return token;
}
