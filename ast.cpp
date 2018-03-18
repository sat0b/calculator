#include "ast.h"
#include "token.h"

SymbolAst::SymbolAst(Token tk) : token(std::move(tk)) {}
TokenKind SymbolAst::get_type() { return Symbol; }

IntAst::IntAst(Token tk) : token(std::move(tk)) {}
TokenKind IntAst::get_type() { return Integer; }
int IntAst::get_value() { return token.get_value(); }

AssignAst::AssignAst(Ast *d, Ast *s) : dst(d), src(s) {}
TokenKind AssignAst::get_type() { return Assign; }

PrintAst::PrintAst(Ast *_ast) : ast(_ast) {}
TokenKind PrintAst::get_type() { return Print; }

ExprAst::ExprAst(Ast *_left, Ast *_right, Token _token)
    : left(_left), right(_right), token(std::move(_token)) {}
TokenKind ExprAst::get_type() { return Expr; }

ForAst::ForAst(Ast *_cond, Ast *_block) : cond(_cond), block(_block) {}
TokenKind ForAst::get_type() { return For; }

BlockAst::BlockAst(std::vector<Ast *> _stats) : stats(std::move(_stats)) {}
TokenKind BlockAst::get_type() { return Block; }

IfAst::IfAst(Ast *_cond, Ast *_then_block, Ast *_else_block)
    : cond(_cond), then_block(_then_block), else_block(_else_block) {}
IfAst::IfAst(Ast *_cond, Ast *_then_block)
    : cond(_cond), then_block(_then_block) {}
TokenKind IfAst::get_type() { return If; }

FunctionDefAst::FunctionDefAst(std::string _func_name,
                               std::vector<std::string> _args,
                               std::vector<Ast *> _stats)
    : func_name(std::move(_func_name)), args(std::move(_args)),
      stats(std::move(_stats)) {}
TokenKind FunctionDefAst::get_type() { return FunctionDef; }

FunctionAst::FunctionAst(std::string _func_name, std::vector<Ast *> _args)
    : func_name(std::move(_func_name)), args(std::move(_args)) {}
TokenKind FunctionAst::get_type() { return Function; }

ReturnAst::ReturnAst(Ast *_expr) : expr(_expr) {}
TokenKind ReturnAst::get_type() { return Return; }
