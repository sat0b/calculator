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
    virtual TokenKind get_type();
};

class AssignAst : public Ast {
  public:
    Ast *dst, *src;
    AssignAst(Ast *, Ast *);
    virtual TokenKind get_type();
};

class PrintAst : public Ast {
  public:
    Ast *ast;
    PrintAst(Ast *);
    virtual TokenKind get_type();
};

class IntAst : public Ast {
  public:
    Token token;
    IntAst(Token token);
    int get_value();
    virtual TokenKind get_type();
};

class AddAst : public Ast {
  public:
    Ast *left, *right;
    AddAst(Ast *left, Ast *right);
    virtual TokenKind get_type();
};
