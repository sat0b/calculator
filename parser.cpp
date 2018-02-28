#include "parser.h"
#include "lexer.h"
#include "token.h"
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

void Error::print_error_message() {
    switch (err_type) {
    case ErrorType::SyntaxError:
        std::cerr << "Syntax Error" << std::endl;
        break;
    case ErrorType::NameError:
        std::cerr << "Name Error, no such a variable " << err_message
                  << std::endl;
        break;
    case ErrorType::SymbolError:
        std::cerr << "Symbol Error" << err_message << std::endl;
    default:
        std::cerr << "Error" << std::endl;
        break;
    }
}

void Error::set_syntax_error(std::string msg) {
    err = true;
    err_type = ErrorType::SyntaxError;
    err_message = msg;
}

void Error::set_name_error(std::string msg) {
    err = true;
    err_type = ErrorType::NameError;
    err_message = msg;
}

void Error::set_symbol_error(std::string msg) {
    err = true;
    err_type = ErrorType::SymbolError;
    err_message = msg;
}

void Error::reset() { err = false; }

bool Error::state() { return err; }

Parser::Parser(Lexer *lexer) : lexer(lexer) {}

void Parser::variable_statement() {
    Token token = lexer->next_token();
    std::string name = token.get_name();
    if (lexer->skip(Assign)) {
        expression(1);
        if (stack.exist())
            variables[name] = stack.pop();
        else
            error.set_syntax_error("");
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
    if (error.state())
        return;
    lexer->match(StatementEnd);
    if (error.state())
        return;
    if (stack.exist())
        std::cout << stack.pop() << std::endl;
    else
        error.set_syntax_error("");
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
    if (stack.exist()) {
        val = stack.pop();
    } else {
        error.set_syntax_error("");
        return;
    }
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
    if (error.state())
        return;
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
        error.set_syntax_error("");
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
        if (!success) {
            error.set_syntax_error("");
            return;
        }
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
            error.set_name_error(token.get_name());
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
    error.reset();
    stack.clear();
    // lexer->show_tokens();

    for (;;) {
        if (lexer->match(CodeEnd))
            break;
        statement();
        if (error.state()) {
            error.print_error_message();
            return;
        }
        lexer->skip(StatementEnd);
    }
}
