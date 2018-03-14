#include "runner.h"
#include <iostream>

Runner::Runner(std::vector<Ast *> _astvec) : astvec(_astvec) {}

void Runner::run() {
    for (Ast *ast : astvec)
        run(ast);
}

void Runner::run(Ast *ast) {
    switch (ast->get_type()) {
    case Assign:
        run(dynamic_cast<AssignAst *>(ast));
        break;
    case Integer:
        run(dynamic_cast<IntAst *>(ast));
        break;
    case Symbol:
        run(dynamic_cast<SymbolAst *>(ast));
        break;
    case Print:
        run(dynamic_cast<PrintAst *>(ast));
        break;
    case Expr:
        run(dynamic_cast<ExprAst *>(ast));
        break;
    case For:
        run(dynamic_cast<ForAst *>(ast));
        break;
    default:
        break;
    }
}

void Runner::run(AssignAst *ast) {
    SymbolAst *sym_ast = dynamic_cast<SymbolAst *>(ast->dst);
    run(ast->src);
    global_var[sym_ast->token.get_name()] = stack.pop();
}

void Runner::run(PrintAst *ast) {
    run(ast->ast);
    std::cout << stack.pop() << std::endl;
}

void Runner::run(ExprAst *ast) {
    run(ast->left);
    run(ast->right);
    stack.operate(ast->token.get_kind());
}

void Runner::run(IntAst *ast) { stack.push(ast->get_value()); }

void Runner::run(SymbolAst *ast) {
    std::string name = ast->token.get_name();
    if (global_var.count(name) > 0)
        stack.push(global_var[name]);
    else
        std::cerr << "Name error, no such a variable " + name << std::endl;
}

void Runner::run(ForAst *ast) {
    for (;;) {
        run(ast->cond);
        if (!stack.pop())
            break;
        for (Ast *stat : ast->block)
            run(stat);
    }
}
