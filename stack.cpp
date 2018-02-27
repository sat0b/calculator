#include "stack.h"

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
