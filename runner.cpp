#include "runner.h"
#include <algorithm>
#include <numeric>

Runner::Runner(std::vector<Ast *> _astvec) : astvec(std::move(_astvec)) {}

void Runner::run() {
    make_builtin_function();
    for (Ast *ast : astvec)
        run(ast);
}

std::vector<int> convert_args(std::vector<Ast *> args) {
    std::vector<int> iargs;
    for (Ast *arg : args) {
        int value = dynamic_cast<IntAst *>(arg)->get_value();
        iargs.push_back(value);
    }
    return iargs;
}

int builtin_max(std::vector<Ast *> args) {
    std::vector<int> iargs = convert_args(args);
    return *std::max_element(iargs.begin(), iargs.end());
}

int builtin_min(std::vector<Ast *> args) {
    std::vector<int> iargs = convert_args(args);
    return *std::min_element(iargs.begin(), iargs.end());
}

int builtin_sum(std::vector<Ast *> args) {
    std::vector<int> iargs = convert_args(args);
    return std::accumulate(iargs.begin(), iargs.end(), 0);
}

void Runner::make_builtin_function() {
    builtin_function_table["max"] = builtin_max;
    builtin_function_table["min"] = builtin_min;
    builtin_function_table["sum"] = builtin_sum;
}

void Runner::run(Ast *ast) {
    switch (ast->get_type()) {
    case Assign:
        run(dynamic_cast<AssignAst *>(ast));
        break;
    case Integer:
        run(dynamic_cast<IntAst *>(ast));
        break;
    case String:
        run(dynamic_cast<StringAst *>(ast));
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
    case If:
        run(dynamic_cast<IfAst *>(ast));
        break;
    case Block:
        for (Ast *stat : dynamic_cast<BlockAst *>(ast)->stats)
            run(stat);
        break;
    case FunctionDef:
        run(dynamic_cast<FunctionDefAst *>(ast));
        break;
    case Function:
        run(dynamic_cast<FunctionAst *>(ast));
        break;
    case Return:
        run(dynamic_cast<ReturnAst *>(ast));
        break;
    default:
        break;
    }
}

void Runner::run(AssignAst *ast) {
    run(ast->src);
    auto sym_ast = dynamic_cast<SymbolAst *>(ast->dst);
    global_var[sym_ast->token.get_name()] = stack.pop();
}

void Runner::run(PrintAst *print_ast) {
    if (print_ast->ast->get_type() == String) {
        auto str = dynamic_cast<StringAst *>(print_ast->ast);
        std::cout << str->token.get_name() << std::endl;
    } else {
        run(print_ast->ast);
        std::cout << stack.pop() << std::endl;
    }
}

void Runner::run(ExprAst *ast) {
    run(ast->left);
    run(ast->right);
    stack.operate(ast->token.get_kind());
}

void Runner::run(IntAst *ast) { stack.push(ast->get_value()); }

void Runner::run(StringAst *ast) {}

void Runner::run(SymbolAst *ast) {
    std::string name = ast->token.get_name();
    if (!scope.empty()) {
        std::map<std::string, int> local_var = scope.top();
        if (local_var.count(name) > 0) {
            stack.push(local_var[name]);
            return;
        }
    }
    if (global_var.count(name) > 0)
        stack.push(global_var[name]);
    else
        std::cerr << "Name error, no such a variable " + name << std::endl;
}

void Runner::run(ForAst *ast) {
    // range based for loop
    if (ast->range_loop) {
        std::map<std::string, int> local_var;
        SymbolAst *val = ast->val;
        for (int i = ast->begin; i <= ast->end; ++i) {
            local_var[val->token.get_name()] = i;
            scope.push(local_var);
            run(ast->block);
            scope.pop();
        }
    }
    // condition based for loop
    else {
        for (;;) {
            run(ast->cond);
            if (!stack.pop())
                break;
            run(ast->block);
        }
    }
}

void Runner::run(IfAst *ast) {
    run(ast->cond);
    if (stack.pop())
        run(ast->then_block);
    else
        run(ast->else_block);
}

void Runner::run(FunctionDefAst *ast) { function_table[ast->func_name] = ast; }

void Runner::run(FunctionAst *ast) {
    std::string func_name = ast->func_name;
    std::vector<Ast *> args_value = ast->args;
    // builtin function
    if (builtin_function_table.count(func_name)) {
        int value = builtin_function_table[func_name](args_value);
        stack.push(value);
        return;
    }
    FunctionDefAst *func_ast = function_table[func_name];
    if (args_value.size() != func_ast->args.size())
        std::cerr << "Syntax error, the number of arguments do not match"
                  << std::endl;
    std::map<std::string, int> local_var;
    for (int i = 0; i < args_value.size(); i++) {
        run(args_value[i]);
        int arg_value = stack.pop();
        local_var[func_ast->args[i]] = arg_value;
    }
    scope.push(local_var);
    for (Ast *stat : func_ast->stats)
        run(stat);
    scope.pop();
}

void Runner::run(ReturnAst *ast) {
    // TODO
    run(ast->expr);
}