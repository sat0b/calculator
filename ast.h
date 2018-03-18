#pragma once
#include "token.h"

class Ast {
  public:
    virtual TokenKind get_type() = 0;
};

class SymbolAst : public Ast {
  public:
    Token token;
    explicit SymbolAst(Token tk);
    TokenKind get_type() override;
};

class AssignAst : public Ast {
  public:
    Ast *dst, *src;
    AssignAst(Ast *, Ast *);
    TokenKind get_type() override;
};

class PrintAst : public Ast {
  public:
    Ast *ast;
    explicit PrintAst(Ast *);
    TokenKind get_type() override;
};

class IntAst : public Ast {
  public:
    Token token;
    explicit IntAst(Token token);
    int get_value();
    TokenKind get_type() override;
};

class ExprAst : public Ast {
  public:
    Ast *left, *right;
    Token token;
    ExprAst(Ast *, Ast *, Token);
    TokenKind get_type() override;
};

class ForAst : public Ast {
  public:
    Ast *cond;
    Ast *block;
    ForAst(Ast *, Ast *);
    TokenKind get_type() override;
};

class BlockAst : public Ast {
  public:
    std::vector<Ast *> stats;
    explicit BlockAst(std::vector<Ast *>);
    TokenKind get_type() override;
};

class IfAst : public Ast {
  public:
    Ast *cond;
    Ast *then_block;
    Ast *else_block;
    IfAst(Ast *, Ast *, Ast *);
    IfAst(Ast *, Ast *);
    TokenKind get_type() override;
};

class FunctionDefAst : public Ast {
  public:
    std::string func_name;
    std::vector<std::string> args;
    std::vector<Ast *> stats;
    FunctionDefAst(std::string func_name, std::vector<std::string> args,
                   std::vector<Ast *> stats);
    TokenKind get_type() override;
};

class FunctionAst : public Ast {
  public:
    std::vector<Ast *> args;
    std::string func_name;
    FunctionAst(std::string func_name, std::vector<Ast *> args);
    TokenKind get_type() override;
};

class ReturnAst : public Ast {
  public:
    Ast *expr;
    explicit ReturnAst(Ast *expr);
    TokenKind get_type() override;
};
