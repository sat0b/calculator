#include "parser.h"
#include <iostream>
#include <numeric>

std::map<TokenKind, int> Parser::exp_order{
    {Or, 1},       {And, 2},       {Equal, 3},       {NotEqual, 3},
    {LessThan, 4}, {LessEqual, 4}, {GreaterThan, 4}, {GreaterEqual, 4},
    {Plus, 5},     {Minus, 5},     {Product, 6},     {Divide, 6},
    {Mod, 6},
};

Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer(std::move(lexer)) {}

Ast *Parser::read_symbol_stat() {
    SymbolAst *sym = new SymbolAst(lexer->next_token());
    lexer->expect_skip(Assign);
    Ast *expr = read_expr(1);
    return new AssignAst(sym, expr);
}

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
    return nullptr;
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

std::vector<Ast *> Parser::parse() {
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
