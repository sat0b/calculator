#include "stack.h"
#include <cstdlib>
#include <iostream>

void Stack::push(int value) { stack.push(value); }

int Stack::pop() {
    int top = stack.top();
    stack.pop();
    return top;
}

void Stack::clear() {
    while (stack.size() > 0) {
        stack.pop();
    }
}

bool Stack::exist() {
    if (stack.size() > 0)
        return true;
    else
        return false;
}

void Stack::operate(const TokenKind op) {
    if (stack.size() < 2)
        error("Stack underflow");
    int d2 = pop();
    int d1 = pop();
    if (op == Plus)
        push(d1 + d2);
    else if (op == Minus)
        push(d1 - d2);
    else if (op == Product)
        push(d1 * d2);
    else if (op == Divide)
        push(d1 / d2);
    else if (op == And)
        push(d1 && d2);
    else if (op == Or)
        push(d1 || d2);
    else if (op == Equal)
        push(d1 == d2);
    else if (op == NotEqual)
        push(d1 != d2);
    else if (op == LessThan)
        push(d1 < d2);
    else if (op == LessEqual)
        push(d1 <= d2);
    else if (op == GreaterThan)
        push(d1 > d2);
    else if (op == GreaterEqual)
        push(d1 >= d2);
    else if (op == Mod)
        push(d1 % d2);
    else
        error("Not defined operator");
}

void Stack::error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(1);
}
