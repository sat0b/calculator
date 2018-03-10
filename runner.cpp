#include "runner.h"
#include <iostream>

// Runner::Runner(std::vector<Ast *> _astvec) : astvec(_astvec) {}

// void Runner::run() {
//     for (Ast *ast : astvec)
//         run(ast);
// }

// void Runner::run(Ast *ast) {
//     switch (ast->get_type()) {
//     case Equal:
//         run((AssignAst *)ast);
//         break;
//     case Integer:
//         run((IntAst *)ast);
//         break;
//     case Print:
//         run((PrintAst *)ast);
//         break;
//     default:
//         break;
//     }
// }
//
// void Runner::run(AssignAst *ast) {
//     SymbolAst *sym_ast = (SymbolAst *)ast->dst;
//     IntAst *int_ast = (IntAst *)ast->src;
//     global_var[sym_ast->token.get_name()] = int_ast->token.get_value();
// }
//
// void Runner::run(PrintAst *ast) {
//     SymbolAst *sym_ast = (SymbolAst *)ast;
//     std::cout << global_var[sym_ast->token.get_name()] << std::endl;
// }
//
// void Runner::run(IntAst *ast) { return; }
