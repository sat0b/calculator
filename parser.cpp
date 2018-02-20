#include "parser.h"
#include <iostream>

void Error::printErrorMessage() {
  switch (errType) {
  case ErrorType::SyntaxError:
    std::cerr << "Syntax Error" << std::endl;
    break;
  case ErrorType::NameError:
    std::cerr << "Name Error, no such a variable " << errMessage << std::endl;
    break;
  case ErrorType::SymbolError:
    std::cerr << "Symbol Error" << errMessage << std::endl;
  default:
    std::cerr << "Error" << std::endl;
    break;
  }
}

void Error::setSyntaxError(std::string msg) {
  err = true;
  errType = ErrorType::SyntaxError;
  errMessage = msg;
}

void Error::setNameError(std::string msg) {
  err = true;
  errType = ErrorType::NameError;
  errMessage = msg;
}

void Error::setSymbolError(std::string msg) {
  err = true;
  errType = ErrorType::SymbolError;
  errMessage = msg;
}

void Error::reset() { err = false; }

bool Error::state() { return err; }

void Parser::next() { token = tokenizer.nextToken(); }

void Parser::checkKind(const TokenKind kind) {
  if (token.getKind() != kind)
    error.setSyntaxError("");
}

void Parser::statement() {
  if (error.state())
    return;
  if (token.getKind() == Variable) {
    std::string name = token.getName();
    next();
    if (token.getKind() == Assign) {
      next();
      orExpression();
      if (error.state())
        return;
      checkKind(StatementEnd);
      if (error.state())
        return;
      if (stack.size() > 0) {
        variables[name] = stack.top();
        stack.pop();
      } else {
        error.setSyntaxError("");
      }
    } else if (token.getKind() == StatementEnd) {
      if (replMode) {
        if (variables.count(name) > 0) {
          std::cout << variables[name] << std::endl;
        } else {
          error.setNameError(name);
        }
      } else {
        error.setSyntaxError("");
      }
    } else {
      error.setSyntaxError("");
    }
  } else if (token.getKind() == Print) {
    next();
    if (token.getKind() != Variable && token.getKind() != Integer) {
      error.setSyntaxError("");
      return;
    }
    orExpression();
    if (error.state())
      return;
    if (stack.size() > 0) {
      std::cout << stack.top() << std::endl;
      stack.pop();
    } else {
      error.setSyntaxError("");
    }
  } else {
    error.setSyntaxError("");
  }
}

void Parser::orExpression() {
  andExpression();
  while (token.getKind() == Or) {
    TokenKind op = token.getKind();
    next();
    andExpression();
    operate(op);
  }
}

void Parser::andExpression() {
  equalExpression();
  while (token.getKind() == And) {
    TokenKind op = token.getKind();
    next();
    equalExpression();
    operate(op);
  }
}

void Parser::equalExpression() {
  thanExpression();
  while (token.getKind() == Equal || token.getKind() == NotEqual) {
    TokenKind op = token.getKind();
    next();
    thanExpression();
    operate(op);
  }
}

void Parser::thanExpression() {
  expression();
  while (token.getKind() == LessThan || token.getKind() == LessEqual ||
         token.getKind() == GreaterThan || token.getKind() == GreaterEqual) {
    TokenKind op = token.getKind();
    next();
    expression();
    operate(op);
  }
}

void Parser::expression() {
  term();
  while (token.getKind() == Plus || token.getKind() == Minus) {
    TokenKind op = token.getKind();
    next();
    term();
    operate(op);
  }
}

void Parser::term() {
  factor();
  while (token.getKind() == Product || token.getKind() == Divide ||
         token.getKind() == Mod) {
    TokenKind op = token.getKind();
    next();
    factor();
    operate(op);
  }
}

void Parser::factor() {
  switch (token.getKind()) {
  case Integer:
    stack.push(token.getValue());
    break;
  case Variable:
    if (variables.count(token.getName()) > 0) {
      stack.push(variables[token.getName()]);
    } else {
      error.setNameError(token.getName());
      return;
    }
    break;
  case LeftBracket:
    next();
    expression();
    checkKind(RightBracket);
    if (error.state())
      return;
    break;
  default:
    break;
  }
  next();
}

void Parser::operate(const TokenKind op) {
  int d2 = stack.top();
  stack.pop();
  int d1 = stack.top();
  stack.pop();

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
    std::string msg = "Not Defined operator : " + getTokenString(op);
    error.setSymbolError(msg);
    break;
  }
}

void Parser::showVariableTable() const {
  for (auto v : variables) {
    std::cout << v.first << " : " << v.second << std::endl;
  }
}

void Parser::run(const std::string &line, bool replMode) {
  tokenizer.init(line);
  error.reset();
  // tokenizer.showTokens();
  this->replMode = replMode;

  while (true) {
    next();
    statement();
    if (error.state()) {
      error.printErrorMessage();
      return;
    }
    if (token.getKind() == StatementEnd) {
      return;
    } else {
      error.printErrorMessage();
      return;
    }
  }
}
