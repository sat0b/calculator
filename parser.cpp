#include "parser.h"
#include <iostream>

void Parser::next() { token = tokenizer.nextToken(); }

void Parser::checkKind(const TokenKind kind) {
  if (token.getKind() != kind)
    err = true;
}

void Parser::statement() {
  if (token.getKind() == Variable) {
    std::string name = token.getName();
    next();
    checkKind(Assign);
    if (err)
      return;
    next();
    orExpression();
    variables[name] = stack.top();
    stack.pop();
  } else if (token.getKind() == Print) {
    next();
    checkKind(Variable);
    if (err)
      return;
    // Get variable name
    std::string name = token.getName();
    // Check format : print a;
    next();
    checkKind(StatementEnd);
    if (err)
      return;
    // Print variable
    std::cout << variables[name] << std::endl;
  } else {
    err = true;
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
    stack.push(variables[token.getName()]);
    break;
  case LeftBracket:
    next();
    expression();
    checkKind(RightBracket);
    if (err)
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
    std::cerr << "Not Defined operator : " << getTokenString(op) << std::endl;
    err = 1;
    break;
  }
}

void Parser::showVariableTable() const {
  for (auto v : variables) {
    std::cout << v.first << " : " << v.second << std::endl;
  }
}

void Parser::run(const std::string &line) {
  tokenizer.init(line);
  // tokenizer.showTokens();
  err = false;

  while (true) {
    next();
    statement();
    if (err) {
      std::cerr << "Syntax Error " << std::endl;
      return;
    }
    if (token.getKind() == StatementEnd) {
      return;
    } else {
      std::cerr << "Syntax Error" << std::endl;
      return;
    }
  }
}
