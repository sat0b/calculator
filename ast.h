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

class ExprAst : public Ast {
  public:
    Ast *left, *right;
    Token token;
    ExprAst(Ast *, Ast *, Token);
    virtual TokenKind get_type();
};

class ForAst : public Ast {
  public:
    Ast *cond;
    Ast *block;
    ForAst(Ast *, Ast *);
    virtual TokenKind get_type();
};

class BlockAst : public Ast {
  public:
    std::vector<Ast *> stats;
    BlockAst(std::vector<Ast *>);
    virtual TokenKind get_type();
};
