#pragma once
#include "ast.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Runner {
  private:
    std::vector<Ast *> astvec;
    std::map<std::string, int> global_var;

  public:
    Runner(std::vector<Ast *> _astvec);
    void run();
    void run(Ast *ast);
    void run(AssignAst *ast);
    void run(PrintAst *ast);
};
