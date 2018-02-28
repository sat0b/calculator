#include "parser.h"
#include "lexer.h"
#include "token.h"
#include <iostream>

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

void Parser::next() { token = lexer->next_token(); }

void Parser::check_kind(const TokenKind kind) {
    if (token.get_kind() != kind)
        error.set_syntax_error("");
}

void Parser::variable_statement() {
    Token token = lexer->next_token();
    std::string name = token.get_name();
    if (lexer->skip(Assign)) {
        or_expression();
        if (stack.exist())
            variables[name] = stack.pop();
        else
            error.set_syntax_error("");
    }
}

void Parser::print_statement() {
    if (lexer->match(Variable) || lexer->match(Integer)) {
        or_expression();
        if (stack.exist())
            std::cout << stack.pop() << std::endl;
    }
}

void Parser::numeric_statement() {
    or_expression();
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
    or_expression();
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
        skip_until(ElseIf);
        skip_until(Else);
    }
}

void Parser::else_if_statement() {}

void Parser::else_statement() {}

void Parser::skip_until(TokenKind kind) {}

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

void Parser::or_expression() {
    and_expression();
    while (lexer->match(Or)) {
        TokenKind op = lexer->next_token().get_kind();
        and_expression();
        operate(op);
    }
}

void Parser::and_expression() {
    equal_expression();
    while (lexer->match(And)) {
        TokenKind op = lexer->next_token().get_kind();
        equal_expression();
        operate(op);
    }
}

void Parser::equal_expression() {
    than_expression();
    while (lexer->match(Equal) || lexer->match(NotEqual)) {
        TokenKind op = lexer->next_token().get_kind();
        than_expression();
        operate(op);
    }
}

void Parser::than_expression() {
    expression();
    while (lexer->match(LessThan) || lexer->match(LessEqual) ||
           lexer->match(GreaterThan) || lexer->match(GreaterEqual)) {
        TokenKind op = lexer->next_token().get_kind();
        expression();
        operate(op);
    }
}

void Parser::expression() {
    term();
    while (lexer->match(Plus) || lexer->match(Minus)) {
        TokenKind op = lexer->next_token().get_kind();
        term();
        operate(op);
    }
}

void Parser::term() {
    factor();
    while (lexer->match(Product) || lexer->match(Divide) || lexer->match(Mod)) {
        TokenKind op = lexer->next_token().get_kind();
        factor();
        operate(op);
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
        expression();
        lexer->skip(RightBracket);
        break;
    default:
        break;
    }
}

void Parser::operate(const TokenKind op) {
    int d1, d2;
    if (!stack.exist())
        return;
    d2 = stack.pop();
    if (!stack.exist())
        return;
    d1 = stack.pop();

    switch (op) {
    case Plus:
        stack.push(d1 + d2);
        break;
    case Minus:
        stack.push(d1 - d2);
        break;
    case Product:
        stack.push(d1 * d2);
        break;
    case Divide:
        stack.push(d1 / d2);
        break;
    case And:
        stack.push(d1 && d2);
        break;
    case Or:
        stack.push(d1 || d2);
        break;
    case Equal:
        stack.push(d1 == d2);
        break;
    case NotEqual:
        stack.push(d1 != d2);
        break;
    case LessThan:
        stack.push(d1 < d2);
        break;
    case LessEqual:
        stack.push(d1 <= d2);
        break;
    case GreaterThan:
        stack.push(d1 > d2);
        break;
    case GreaterEqual:
        stack.push(d1 >= d2);
        break;
    case Mod:
        stack.push(d1 % d2);
        break;
    default:
        std::string msg = "Not Defined operator";
        error.set_symbol_error(msg);
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
    // this->repl_mode = repl_mode;

    while (!lexer->match(CodeEnd)) {
        statement();
        if (error.state()) {
            error.print_error_message();
            return;
        }
        lexer->skip(StatementEnd);
    }
}
