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
    std::string name = token.get_name();
    next();
    if (token.get_kind() == Assign) {
        next();
        or_expression();
        if (error.state())
            return;
        check_kind(StatementEnd);
        if (error.state())
            return;
        if (stack.exist())
            variables[name] = stack.pop();
        else
            error.set_syntax_error("");
    } else if (token.get_kind() == StatementEnd) {
        if (repl_mode) {
            if (variables.count(name) > 0) {
                std::cout << variables[name] << std::endl;
            } else {
                error.set_name_error(name);
            }
        } else {
            error.set_syntax_error("");
        }
    } else {
        error.set_syntax_error("");
    }
}

void Parser::print_statement() {
    next();
    if (token.get_kind() != Variable && token.get_kind() != Integer) {
        error.set_syntax_error("");
        return;
    }
    or_expression();
    if (error.state())
        return;
    check_kind(StatementEnd);
    if (error.state())
        return;
    if (stack.exist())
        std::cout << stack.pop() << std::endl;
    else
        error.set_syntax_error("");
}

void Parser::numeric_statement() {
    or_expression();
    if (error.state())
        return;
    check_kind(StatementEnd);
    if (error.state())
        return;
    if (stack.exist())
        std::cout << stack.pop() << std::endl;
    else
        error.set_syntax_error("");
}

void Parser::block() {
    next();
    if (error.state())
        return;
    while (token.get_kind() != ElseIf && token.get_kind() != Else &&
           token.get_kind() != End && token.get_kind() != Break &&
           token.get_kind() != Return) {
        statement();
        next();
    }
}

void Parser::for_statement() {}

void Parser::if_statement() {
    if (!lexer->skip(LeftBracket))
        return;
    next();
    or_expression();
    if (error.state())
        return;
    check_kind(RightBracket);
    if (error.state())
        return;

    // check truth
    int val = 0;
    if (stack.exist()) {
        val = stack.pop();
    } else {
        error.set_syntax_error("");
        return;
    }

    if (!lexer->skip(StatementEnd))
        return;

    if (val) {
        block();
        next();
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
    if (token.get_kind() == Variable)
        variable_statement();
    else if (token.get_kind() == Print)
        print_statement();
    else if (token.get_kind() == Integer)
        numeric_statement();
    else if (token.get_kind() == If || token.get_kind() == ElseIf ||
             token.get_kind() == Else)
        if_statement();
    else if (token.get_kind() == For)
        for_statement();
    else
        error.set_syntax_error("");
}

void Parser::or_expression() {
    and_expression();
    while (token.get_kind() == Or) {
        TokenKind op = token.get_kind();
        next();
        and_expression();
        operate(op);
    }
}

void Parser::and_expression() {
    equal_expression();
    while (token.get_kind() == And) {
        TokenKind op = token.get_kind();
        next();
        equal_expression();
        operate(op);
    }
}

void Parser::equal_expression() {
    than_expression();
    while (token.get_kind() == Equal || token.get_kind() == NotEqual) {
        TokenKind op = token.get_kind();
        next();
        than_expression();
        operate(op);
    }
}

void Parser::than_expression() {
    expression();
    while (token.get_kind() == LessThan || token.get_kind() == LessEqual ||
           token.get_kind() == GreaterThan ||
           token.get_kind() == GreaterEqual) {
        TokenKind op = token.get_kind();
        next();
        expression();
        operate(op);
    }
}

void Parser::expression() {
    term();
    while (token.get_kind() == Plus || token.get_kind() == Minus) {
        TokenKind op = token.get_kind();
        next();
        term();
        operate(op);
    }
}

void Parser::term() {
    factor();
    while (token.get_kind() == Product || token.get_kind() == Divide ||
           token.get_kind() == Mod) {
        TokenKind op = token.get_kind();
        next();
        factor();
        operate(op);
    }
}

void Parser::factor() {
    switch (token.get_kind()) {
    case Integer:
        stack.push(token.get_value());
        break;
    case Variable:
        if (variables.count(token.get_name()) > 0) {
            stack.push(variables[token.get_name()]);
        } else {
            error.set_name_error(token.get_name());
            return;
        }
        break;
    case LeftBracket:
        next();
        expression();
        check_kind(RightBracket);
        if (error.state())
            return;
        break;
    default:
        break;
    }
    next();
}

void Parser::operate(const TokenKind op) {
    int d1, d2;
    if (stack.exist()) {
        d2 = stack.pop();
    } else {
        error.set_syntax_error("");
        return;
    }
    if (stack.exist()) {
        d1 = stack.pop();
    } else {
        error.set_syntax_error("");
        return;
    }

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
    for (auto v : variables) {
        std::cout << v.first << " : " << v.second << std::endl;
    }
}

void Parser::run() {
    error.reset();
    stack.clear();
    // lexer->show_tokens();
    // this->repl_mode = repl_mode;

    while (true) {
        // if (lexer->skip(CodeEnd))
        next();
        if (token.get_kind() == CodeEnd)
            break;
        statement();
        if (error.state()) {
            error.print_error_message();
            return;
        }
        if (token.get_kind() != StatementEnd) {
            error.print_error_message();
            return;
        }
    }
}
