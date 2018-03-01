#include "parser.h"
#include "lexer.h"
#include "token.h"
#include <cstdlib>
#include <iostream>

namespace {

std::map<TokenKind, int> exp_order{
    {Or, 1},       {And, 2},       {Equal, 3},       {NotEqual, 3},
    {LessThan, 4}, {LessEqual, 4}, {GreaterThan, 4}, {GreaterEqual, 4},
    {Plus, 5},     {Minus, 5},     {Product, 6},     {Divide, 6},
    {Mod, 6},
};

const int order_max = 6;

} // namespace

Parser::Parser(Lexer *lexer) : lexer(lexer) {}

void Parser::variable_statement() {
    Token token = lexer->next_token();
    std::string name = token.get_name();
    if (lexer->skip(Assign)) {
        expression(1);
        if (stack.exist())
            variables[name] = stack.pop();
        else
            parse_error("Syntax error");
    }
}

void Parser::print_statement() {
    if (lexer->match(Variable) || lexer->match(Integer)) {
        expression(1);
        if (stack.exist())
            std::cout << stack.pop() << std::endl;
    }
}

void Parser::numeric_statement() {
    expression(1);
    lexer->match(StatementEnd);
    if (stack.exist())
        std::cout << stack.pop() << std::endl;
    else
        parse_error("Syntax error");
}

void Parser::block() {
    for (;;) {
        if (lexer->skip(ElseIf) || lexer->skip(Else) || lexer->skip(End) ||
            lexer->skip(Break) || lexer->skip(Return))
            return;
        statement();
        lexer->skip(StatementEnd);
    }
}

void Parser::for_statement() {}

void Parser::if_statement() {
    lexer->skip(LeftBracket);
    expression(1);
    lexer->skip(RightBracket);

    int val = 0;
    if (stack.exist())
        val = stack.pop();
    else
        parse_error("Syntax error");
    lexer->skip(StatementEnd);

    if (val) {
        block();
    } else {
        // jump statement
        // skip_until(ElseIf);
        // skip_until(Else);
    }
}

void Parser::else_if_statement() {}

void Parser::else_statement() {}

void Parser::statement() {
    if (lexer->match(Variable))
        variable_statement();
    else if (lexer->skip(Print))
        print_statement();
    else if (lexer->skip(Integer))
        numeric_statement();
    else if (lexer->skip(If))
        if_statement();
    else if (lexer->skip(For))
        for_statement();
    else
        parse_error("Syntax error");
}

void Parser::expression(int priority) {
    if (priority > order_max)
        return factor();

    expression(priority + 1);
    for (;;) {
        TokenKind tk = lexer->read_token().get_kind();
        if (priority != exp_order[tk])
            return;
        TokenKind op = lexer->next_token().get_kind();
        expression(priority + 1);
        bool success = stack.operate(op);
        if (!success)
            parse_error("Syntax error");
    }
}

void Parser::factor() {
    Token token = lexer->next_token();
    switch (token.get_kind()) {
    case Integer:
        stack.push(token.get_value());
        break;
    case Variable:
        if (variables.count(token.get_name()) > 0)
            stack.push(variables[token.get_name()]);
        else
            parse_error("Name error, no such a variable " + token.get_name());
        break;
    case LeftBracket:
        expression(1);
        lexer->skip(RightBracket);
        break;
    default:
        break;
    }
}

void Parser::show_variable_table() const {
    for (auto v : variables)
        std::cout << v.first << " : " << v.second << std::endl;
}

void Parser::run() {
    stack.clear();
    // lexer->show_tokens();

    for (;;) {
        if (lexer->match(CodeEnd))
            break;
        statement();
        lexer->skip(StatementEnd);
    }
}

void Parser::parse_error() { std::exit(1); }

void Parser::parse_error(std::string str) {
    std::cout << str << std::endl;
    parse_error();
}
