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

void Parser::read_variable_stat() {
    Token token = lexer->next_token();
    std::string name = token.get_name();
    if (lexer->skip(Assign)) {
        eval_expression(1);
        if (stack.exist())
            variables[name] = stack.pop();
        else
            parse_error("Syntax error");
    }
}

void Parser::read_print_stat() {
    if (lexer->match(Variable) || lexer->match(Integer)) {
        eval_expression(1);
        if (stack.exist())
            std::cout << stack.pop() << std::endl;
    }
}

void Parser::read_numeric_stat() {
    eval_expression(1);
    lexer->match(StatementEnd);
    if (stack.exist())
        std::cout << stack.pop() << std::endl;
    else
        parse_error("Syntax error");
}

void Parser::read_block() {
    for (;;) {
        while (lexer->skip(ElseIf) || lexer->skip(Else))
            lexer->jump_block();
        if (lexer->skip(End))
            return;
        read_stat();
        lexer->skip(StatementEnd);
    }
}

int Parser::read_cond() {
    lexer->skip(LeftBracket);
    eval_expression(1);
    lexer->skip(RightBracket);
    int val = stack.pop();
    lexer->skip(StatementEnd);
    return val;
}

void Parser::read_for_stat() {
    for (;;) {
        int val = read_cond();
        if (!val) {
            lexer->jump_block();
            lexer->skip(End);
            return;
        }
        read_block();
        lexer->jump_back(For);
    }
}

void Parser::read_if_stat() {
    int val = read_cond();
    if (val) {
        read_block();
        return;
    }

    while (lexer->jump_block()) {
        if (lexer->skip(ElseIf)) {
            read_if_stat();
            return;
        } else if (lexer->skip(Else)) {
            lexer->skip(StatementEnd);
            read_block();
            return;
        } else if (lexer->skip(End)) {
            return;
        }
    }
}

void Parser::read_stat() {
    if (lexer->match(Variable))
        read_variable_stat();
    else if (lexer->skip(Print))
        read_print_stat();
    else if (lexer->skip(Integer))
        read_numeric_stat();
    else if (lexer->skip(If))
        read_if_stat();
    else if (lexer->skip(For))
        read_for_stat();
    else
        parse_error("Syntax error");
}

void Parser::eval_expression(int priority) {
    if (priority > order_max)
        return eval_factor();

    eval_expression(priority + 1);
    for (;;) {
        TokenKind tk = lexer->read_token().get_kind();
        if (priority != exp_order[tk])
            return;
        TokenKind op = lexer->next_token().get_kind();
        eval_expression(priority + 1);
        stack.operate(op);
    }
}

void Parser::eval_factor() {
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
        eval_expression(1);
        lexer->skip(RightBracket);
        break;
    default:
        break;
    }
}

void Parser::run() {
    stack.clear();
    // lexer->show_tokens();

    for (;;) {
        if (lexer->match(CodeEnd))
            break;
        read_stat();
        lexer->skip(StatementEnd);
    }
}

void Parser::parse_error(std::string str) {
    std::cout << str << std::endl;
    std::exit(1);
}
