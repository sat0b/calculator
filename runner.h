#pragma once
#include "ast.h"
#include "stack.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Runner {
  private:
    std::vector<Ast *> astvec;
    std::map<std::string, int> global_var;
    Stack stack;

  public:
    Runner(std::vector<Ast *> _astvec);
    void run();
    void run(Ast *ast);
    void run(ExprAst *ast);
    void run(AssignAst *ast);
    void run(PrintAst *ast);
    void run(IntAst *ast);
    void run(SymbolAst *ast);
    void run(ForAst *ast);
    void show_ast(std::vector<Ast *> ast);
};
