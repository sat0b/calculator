#pragma once
#include "token.h"

class Ast {
  public:
    virtual TokenKind get_type() = 0;
};

class SymbolAst : public Ast {
  public:
    Token token;
    SymbolAst(Token tk);
    TokenKind get_type() { return Symbol; }
};

class AssignAst : public Ast {
  public:
    Ast *dst, *src;
    AssignAst(Ast *, Ast *);
    TokenKind get_type() { return Equal; }
};

class PrintAst : public Ast {
  public:
    Ast *ast;
    PrintAst(Ast *);
    TokenKind get_type() { return Print; }
};

class IntAst : public Ast {
  public:
    Token token;
    IntAst(Token token);
    TokenKind get_type() { return Integer; }
};
