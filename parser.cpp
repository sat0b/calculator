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
    Ast *expr = read_expr(1);
    return new AssignAst(sym, expr);
}

// void Parser::read_return_stat() { eval_expression(1); }

// void Parser::call_function(std::string name, std::vector<int> args) {
//     if (name == "max") {
//         int max = *std::max_element(args.begin(), args.end());
//         stack.push(max);
//     } else if (name == "min") {
//         int min = *std::min_element(args.begin(), args.end());
//         stack.push(min);
//     } else if (name == "sum") {
//         int sum = std::accumulate(args.begin(), args.end(), 0);
//         stack.push(sum);
//     }
// }

// void Parser::read_function_call(std::string name) {
//     std::vector<int> vargs;
//     while (!lexer->skip(RightBracket)) {
//         eval_expression(1);
//         int value = stack.pop();
//         lexer->skip(Comma);
//         vargs.push_back(value);
//     }
//     if (std::count(builtins.begin(), builtins.end(), name) > 0) {
//         Parser::call_function(name, vargs);
//         return;
//     }
//     if (functions.count(name) == 0)
//         parse_error("Name error, not defined function " + name);
//     size_t ret_addr = lexer->get_addr();
//     // Process in function
//     lexer->jump_addr(functions[name]);
//     lexer->skip(LeftBracket);
//     std::map<std::string, int> args;
//     int i = 0;
//     while (!lexer->skip(RightBracket)) {
//         std::string value_name = lexer->next_token().get_name();
//         args[value_name] = vargs[i];
//         lexer->skip(Comma);
//         i++;
//     }
//     local_var.push(args);
//     while (!lexer->skip(End))
//         read_stat();
//     local_var.pop();
//     lexer->jump_addr(ret_addr);
// }

Ast *Parser::read_print_stat() {
    Ast *expr = read_expr(1);
    return new PrintAst(expr);
}

Ast *Parser::read_block() {
    std::vector<Ast *> astvec;
    for (;;) {
        if (lexer->skip(End))
            break;
        if (lexer->match(ElseIf) || lexer->match(Else))
            break;
        Ast *ast = read_stat();
        astvec.push_back(ast);
    }
    return new BlockAst(astvec);
}

Ast *Parser::read_for() {
    lexer->expect_skip(LeftBracket);
    Ast *cond = read_expr(1);
    lexer->expect_skip(RightBracket);
    Ast *block = read_block();
    return new ForAst(cond, block);
}

Ast *Parser::read_if() {
    lexer->expect_skip(LeftBracket);
    Ast *cond = read_expr(1);
    lexer->expect_skip(RightBracket);
    Ast *then_block = read_block();
    if (lexer->skip(ElseIf)) {
        Ast *elseif_block = read_if();
        return new IfAst(cond, then_block, elseif_block);
    }
    if (lexer->skip(Else)) {
        Ast *else_block = read_block();
        return new IfAst(cond, then_block, else_block);
    }
    return new IfAst(cond, then_block);
}

Ast *Parser::read_function_def() {
    Token func_token = lexer->next_token();
    std::string func_name = func_token.get_name();
    lexer->expect_skip(LeftBracket);
    std::vector<std::string> args;
    for (;;) {
        Token arg_token = lexer->next_token();
        std::string arg_name = arg_token.get_name();
        args.push_back(arg_name);
        if (lexer->skip(RightBracket))
            break;
        lexer->expect_skip(Comma);
    }
    std::vector<Ast *> stats;
    while (!lexer->skip(End)) {
        Ast *stat = read_stat();
        stats.push_back(stat);
    }
    return new FunctionDefAst(func_name, args, stats);
}

Ast *Parser::read_return() {
    Ast *expr = read_expr(1);
    return new ReturnAst(expr);
}

Ast *Parser::read_stat() {
    if (lexer->match(Symbol))
        return read_symbol_stat();
    if (lexer->skip(Print))
        return read_print_stat();
    if (lexer->skip(For))
        return read_for();
    if (lexer->skip(If))
        return read_if();
    if (lexer->skip(Function))
        return read_function_def();
    if (lexer->skip(Return))
        return read_return();
    parse_error("Syntax error");
}

Ast *Parser::read_expr(int priority) {
    if (priority > order_max)
        return read_factor();
    Ast *node = read_expr(priority + 1);
    for (;;) {
        TokenKind tk = lexer->read_token().get_kind();
        if (priority != exp_order[tk])
            break;
        Token op = lexer->next_token();
        Ast *right = read_expr(priority + 1);
        node = new ExprAst(node, right, op);
    }
    return node;
}

Ast *Parser::read_factor() {
    Token token = lexer->next_token();
    switch (token.get_kind()) {
    case Integer:
        return new IntAst(token);
    case Symbol: {
        if (lexer->skip(LeftBracket)) {
            std::string func_name = token.get_name();
            std::vector<int> args;
            for (;;) {
                Token arg = lexer->next_token();
                args.push_back(arg.get_value());
                if (lexer->skip(RightBracket))
                    break;
                lexer->expect_skip(Comma);
            }

            return new FunctionAst(func_name, args);
        }
        return new SymbolAst(token);
    }
    case LeftBracket: {
        Ast *ast = read_expr(1);
        lexer->expect_skip(RightBracket);
        return ast;
    }
    default:
        return nullptr;
    }
}

// void Parser::eval_expression(int priority) {
//     if (priority > order_max)
//         return eval_factor();
//     eval_expression(priority + 1);
//     for (;;) {
//         TokenKind tk = lexer->read_token().get_kind();
//         if (priority != exp_order[tk])
//             return;
//         TokenKind op = lexer->next_token().get_kind();
//         eval_expression(priority + 1);
//         stack.operate(op);
//     }
// }

// void Parser::eval_factor() {
//     Token token = lexer->next_token();
//     switch (token.get_kind()) {
//     case Integer:
//         stack.push(token.get_value());
//         break;
//     case Symbol: {
//         std::string name = token.get_name();
//         // Load argments
//         std::map<std::string, int> args;
//         if (local_var.size() > 0)
//             args = local_var.top();
//         // Function call
//         if (lexer->skip(LeftBracket)) {
//             read_function_call(name);
//             return;
//         }
//         if (args.count(name) > 0)
//             stack.push(args[name]);
//         else if (global_var.count(name) > 0)
//             stack.push(global_var[name]);
//         else
//             parse_error("Name error, no such a variable " +
//             token.get_name());
//         break;
//     }
//     case LeftBracket:
//         eval_expression(1);
//         lexer->skip(RightBracket);
//         break;
//     default:
//         break;
//     }
// }

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
