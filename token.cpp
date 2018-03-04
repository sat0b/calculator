#include "token.h"
#include <iostream>

TokenKind Token::get_token_kind(const std::string &token) const {
    if (all_of(token.cbegin(), token.cend(), isdigit))
        return Integer;
    if (token == "+")
        return Plus;
    if (token == "-")
        return Minus;
    if (token == "*")
        return Product;
    if (token == "/")
        return Divide;
    if (token == "(")
        return LeftBracket;
    if (token == ")")
        return RightBracket;
    if (token == "=")
        return Assign;
    if (token == "&&")
        return And;
    if (token == "||")
        return Or;
    if (token == "==")
        return Equal;
    if (token == "!=")
        return NotEqual;
    if (token == "<")
        return LessThan;
    if (token == "<=")
        return LessEqual;
    if (token == ">")
        return GreaterThan;
    if (token == ">=")
        return GreaterEqual;
    if (token == "%")
        return Mod;
    if (token == "print")
        return Print;
    if (token == "for")
        return For;
    if (token == "while")
        return While;
    if (token == "if")
        return If;
    if (token == "elseif")
        return ElseIf;
    if (token == "else")
        return Else;
    if (token == "break")
        return Break;
    if (token == "return")
        return Return;
    if (token == "function")
        return Function;
    if (token == ",")
        return Comma;
    if (token == "end")
        return End;
    return Symbol;
}

Token::Token(std::string str, TokenKind token_kind)
    : token(str), kind(token_kind) {}

Token::Token(std::string str) {
    this->kind = get_token_kind(str);
    this->token = str;
}

Token::Token(TokenKind kind) {
    this->kind = kind;
    this->token = "";
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
