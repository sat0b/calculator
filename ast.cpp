#include "ast.h"
#include "token.h"

SymbolAst::SymbolAst(Token tk) : token(tk) {}
TokenKind SymbolAst::get_type() { return Symbol; }

IntAst::IntAst(Token tk) : token(tk) {}
TokenKind IntAst::get_type() { return Integer; }
int IntAst::get_value() { return token.get_value(); }

AssignAst::AssignAst(Ast *d, Ast *s) : dst(d), src(s) {}
TokenKind AssignAst::get_type() { return Equal; }

PrintAst::PrintAst(Ast *_expr) : expr(_expr) {}
TokenKind PrintAst::get_type() { return Print; }

AddAst::AddAst(Ast *_left, Ast *_right) : left(_left), right(_right) {}
TokenKind AddAst::get_type() { return Plus; }

ExprAst::ExprAst(Ast *_left, Ast *_right, Token _token)
    : left(_left), right(_right), token(_token) {}
TokenKind ExprAst::get_type() { return Expr; }
