#include "runner.h"
#include <iostream>

Runner::Runner(std::vector<Ast *> _astvec) : astvec(_astvec) {}

void Runner::run() {
    for (Ast *ast : astvec)
        run(ast);
}

void Runner::run(Ast *ast) {
    switch (ast->get_type()) {
    case Equal:
        run((AssignAst *)ast);
        break;
    case Integer:
        run((IntAst *)ast);
        break;
    case Symbol:
        run((SymbolAst *)ast);
        break;
    case Print:
        run((PrintAst *)ast);
        break;
    case Plus:
        run((AddAst *)ast);
        break;
    default:
        break;
    }
}

void Runner::run(AssignAst *ast) {
    SymbolAst *sym_ast = (SymbolAst *)ast->dst;
    run(ast->src);
    global_var[sym_ast->token.get_name()] = stack.pop();
}

void Runner::run(PrintAst *ast) {
    SymbolAst *sym_ast = dynamic_cast<SymbolAst *>(ast->ast);
    std::cout << global_var[sym_ast->token.get_name()] << std::endl;
}

void Runner::run(AddAst *ast) {
    if (ast->get_type() == Integer) {
        stack.push(dynamic_cast<IntAst *>(ast)->get_value());
        return;
    }
    run(ast->left);
    run(ast->right);
    int right = stack.pop();
    int left = stack.pop();
    stack.push(left + right);
}

void Runner::run(IntAst *ast) { stack.push(ast->get_value()); }

void Runner::run(SymbolAst *ast) {
    std::string name = ast->token.get_name();
    if (global_var.count(name) > 0)
        stack.push(global_var[name]);
    else
        std::cerr << "Not defined symbol " + name << std::endl;
}
