#include "ast.h"
#include "token.h"

SymbolAst::SymbolAst(Token tk) : token(tk) {}
TokenKind SymbolAst::get_type() { return Symbol; }

IntAst::IntAst(Token tk) : token(tk) {}
TokenKind IntAst::get_type() { return Integer; }
int IntAst::get_value() { return token.get_value(); }

AssignAst::AssignAst(Ast *d, Ast *s) : dst(d), src(s) {}
TokenKind AssignAst::get_type() { return Equal; }

PrintAst::PrintAst(Ast *_ast) : ast(_ast) {}
TokenKind PrintAst::get_type() { return Print; }

AddAst::AddAst(Ast *_left, Ast *_right) : left(_left), right(_right) {}
TokenKind AddAst::get_type() { return Plus; }
