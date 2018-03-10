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
    Runner(std::vector<Ast *> _astvec) : astvec(_astvec) {}
    void run() {
        for (Ast *ast : astvec)
            run(ast);
    }
    void run(Ast *ast) {
        switch (ast->get_type()) {
        case Equal:
            run((AssignAst *)ast);
            break;
        case Integer:
            run((IntAst *)ast);
            break;
        case Print:
            run((PrintAst *)ast);
            break;
        default:
            break;
        }
    }
    void run(AssignAst *ast) {
        SymbolAst *sym_ast = (SymbolAst *)ast->dst;
        IntAst *int_ast = (IntAst *)ast->src;
        global_var[sym_ast->token.get_name()] = int_ast->token.get_value();
    }

    void run(PrintAst *ast) {
        SymbolAst *sym_ast = (SymbolAst *)ast;
        std::cout << global_var[sym_ast->token.get_name()] << std::endl;
    }
};
