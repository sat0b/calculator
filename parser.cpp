#include "parser.h"
#include "lexer.h"
#include "token.h"
#include <cstdlib>
#include <iostream>
#include <memory>

namespace {

std::map<TokenKind, int> exp_order{
    {Or, 1},       {And, 2},       {Equal, 3},       {NotEqual, 3},
    {LessThan, 4}, {LessEqual, 4}, {GreaterThan, 4}, {GreaterEqual, 4},
    {Plus, 5},     {Minus, 5},     {Product, 6},     {Divide, 6},
    {Mod, 6},
};

const int order_max = 6;

} // namespace

Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer(std::move(lexer)) {}

void Parser::read_symbol_stat() {
    Token token = lexer->next_token();
    std::string name = token.get_name();
    // Assign statement
    if (lexer->skip(Assign)) {
        eval_expression(1);
        global_var[name] = stack.pop();
    } else if (lexer->skip(Return)) {
        eval_expression(1);
    }
}

void Parser::read_return_stat() { eval_expression(1); }

void Parser::read_function_call(std::string name) {
    std::vector<int> vargs;
    for (;;) {
        if (lexer->skip(RightBracket))
            break;
        int value = lexer->next_token().get_value();
        lexer->skip(Comma);
        vargs.push_back(value);
    }
    if (functions.count(name) == 0)
        parse_error("Name error, not defined function " + name);
    size_t ret_addr = lexer->get_addr();
    // Process in function
    lexer->jump_addr(functions[name]);
    lexer->skip(LeftBracket);
    std::map<std::string, int> args;
    int i = 0;
    while (!lexer->skip(RightBracket)) {
        std::string value_name = lexer->next_token().get_name();
        args[value_name] = vargs[i];
        lexer->skip(Comma);
        i++;
    }
    local_var.push(args);
    lexer->skip(StatementEnd);
    while (!lexer->skip(End)) {
        read_stat();
        lexer->skip(StatementEnd);
    }
    local_var.pop();
    lexer->jump_addr(ret_addr);
}

void Parser::read_print_stat() {
    if (lexer->match(Symbol) || lexer->match(Integer)) {
        eval_expression(1);
        if (stack.exist())
            std::cout << stack.pop() << std::endl;
    }
}

void Parser::read_numeric_stat() {
    eval_expression(1);
    lexer->match(StatementEnd);
    std::cout << stack.pop() << std::endl;
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
        size_t loop_back = lexer->get_addr();
        int val = read_cond();
        if (val) {
            read_block();
            lexer->jump_addr(loop_back);
        } else {
            lexer->jump_block();
            lexer->skip(End);
            return;
        }
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

// Function definition
void Parser::read_function_def() {
    Token token = lexer->next_token();
    std::string name = token.get_name();
    functions[name] = lexer->get_addr();
    lexer->jump_block();
    lexer->skip(End);
}

void Parser::read_stat() {
    if (lexer->match(Symbol))
        read_symbol_stat();
    else if (lexer->skip(Print))
        read_print_stat();
    else if (lexer->skip(Integer))
        read_numeric_stat();
    else if (lexer->skip(If))
        read_if_stat();
    else if (lexer->skip(For))
        read_for_stat();
    else if (lexer->skip(Function))
        read_function_def();
    else if (lexer->skip(Return)) {
        read_return_stat();
    } else {
        parse_error("Syntax error");
    }
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
    case Symbol: {
        std::string name = token.get_name();
        // Load argments
        std::map<std::string, int> args;
        if (local_var.size() > 0)
            args = local_var.top();
        // Function call
        if (lexer->skip(LeftBracket)) {
            read_function_call(name);
            return;
        }
        if (args.count(name) > 0)
            stack.push(args[name]);
        else if (global_var.count(name) > 0)
            stack.push(global_var[name]);
        else
            parse_error("Name error, no such a variable " + token.get_name());
        break;
    }
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
    while (!lexer->match(CodeEnd)) {
        read_stat();
        lexer->skip(StatementEnd);
    }
}

void Parser::parse_error(std::string str) {
    std::cout << str << std::endl;
    std::exit(1);
}
