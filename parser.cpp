#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "token.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>

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

// void Parser::read_symbol_stat() {
//     Token token = lexer->next_token();
//     std::string name = token.get_name();
//     // Assign statement
//     if (lexer->skip(Assign)) {
//         eval_expression(1);
//         global_var[name] = stack.pop();
//     } else if (lexer->skip(Return)) {
//         eval_expression(1);
//     }
// }

Ast *Parser::read_symbol_stat() {
    SymbolAst *sym = new SymbolAst(lexer->next_token());
    lexer->expect_skip(Assign);
    Ast *expr = read_expr();
    return new AssignAst(sym, expr);
}

void Parser::read_return_stat() { eval_expression(1); }

void Parser::call_function(std::string name, std::vector<int> args) {
    if (name == "max") {
        int max = *std::max_element(args.begin(), args.end());
        stack.push(max);
    } else if (name == "min") {
        int min = *std::min_element(args.begin(), args.end());
        stack.push(min);
    } else if (name == "sum") {
        int sum = std::accumulate(args.begin(), args.end(), 0);
        stack.push(sum);
    }
}

void Parser::read_function_call(std::string name) {
    std::vector<int> vargs;
    while (!lexer->skip(RightBracket)) {
        eval_expression(1);
        int value = stack.pop();
        lexer->skip(Comma);
        vargs.push_back(value);
    }
    if (std::count(builtins.begin(), builtins.end(), name) > 0) {
        Parser::call_function(name, vargs);
        return;
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
    while (!lexer->skip(End))
        read_stat();
    local_var.pop();
    lexer->jump_addr(ret_addr);
}

// void Parser::read_print_stat() {
//     if (lexer->match(String)) {
//         std::cout << lexer->next_token().get_name() << std::endl;
//     }
//     if (lexer->match(Symbol) || lexer->match(Integer)) {
//         eval_expression(1);
//         if (stack.exist())
//             std::cout << stack.pop() << std::endl;
//     }
// }

Ast *Parser::read_print_stat() {
    Token token = lexer->next_token();
    SymbolAst *sym = new SymbolAst(token);
    return new PrintAst(sym);
}

void Parser::read_numeric_stat() {
    eval_expression(1);
    std::cout << stack.pop() << std::endl;
}

void Parser::read_block() {
    for (;;) {
        while (lexer->skip(ElseIf) || lexer->skip(Else))
            lexer->jump_block();
        if (lexer->skip(End))
            return;
        if (lexer->skip(Return)) {
            read_return_stat();
            return;
        }
        if (lexer->match(Break)) {
            lexer->jump_end_for();
            break_flg = true;
            return;
        }
        read_stat();
    }
}

int Parser::read_cond() {
    lexer->skip(LeftBracket);
    eval_expression(1);
    lexer->skip(RightBracket);
    return stack.pop();
}

void Parser::read_for_stat() {
    for (;;) {
        size_t loop_back = lexer->get_addr();
        if (read_cond()) {
            read_block();
            if (break_flg) {
                break_flg = false;
                return;
            }
            lexer->jump_addr(loop_back);
        } else {
            lexer->jump_block();
            lexer->skip(End);
            return;
        }
    }
}

void Parser::read_if_stat() {
    if (read_cond()) {
        read_block();
        return;
    }
    while (lexer->jump_block()) {
        if (lexer->skip(ElseIf)) {
            read_if_stat();
            return;
        } else if (lexer->skip(Else)) {
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

Ast *Parser::read_stat() {
    if (lexer->match(Symbol))
        return read_symbol_stat();
    else if (lexer->skip(Print))
        return read_print_stat();
    else if (lexer->skip(Integer))
        read_numeric_stat();
    else if (lexer->skip(If))
        read_if_stat();
    else if (lexer->skip(For))
        read_for_stat();
    else if (lexer->skip(Function))
        read_function_def();
    else if (lexer->skip(Return))
        read_return_stat();
    else
        parse_error("Syntax error");
    return nullptr;
}

Ast *Parser::read_expr() {
    Ast *node = read_factor();
    while (lexer->skip(Plus)) {
        Ast *right = read_factor();
        node = new AddAst(node, right);
    }
    return node;
}

Ast *Parser::read_factor() {
    Token token = lexer->next_token();
    switch (token.get_kind()) {
    case Integer:
        return new IntAst(token);
    case Symbol:
        return new SymbolAst(token);
    default:
        return nullptr;
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

std::vector<Ast *> Parser::parse() {
    stack.clear();
    std::vector<Ast *> astvec;
    while (!lexer->match(CodeEnd)) {
        Ast *ast = read_stat();
        astvec.push_back(ast);
    }
    return astvec;
}

void Parser::parse_error(std::string str) {
    std::cout << str << std::endl;
    std::exit(1);
}
