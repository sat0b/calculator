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
    std::stack<std::map<std::string, int>> scope;
    std::map<std::string, FunctionDefAst *> function_table;
    std::map<std::string, std::function<int(std::vector<Ast *>)>>
        builtin_function_table;
    Stack stack;

  public:
    explicit Runner(std::vector<Ast *> _astvec);
    void run();
    void make_builtin_function();
    void run(Ast *ast);
    void run(ExprAst *ast);
    void run(AssignAst *ast);
    void run(PrintAst *ast);
    void run(IntAst *ast);
    void run(StringAst *ast);
    void run(SymbolAst *ast);
    void run(ForAst *ast);
    void run(IfAst *ast);
    void run(FunctionDefAst *ast);
    void run(FunctionAst *ast);
    void run(ReturnAst *ast);
};
