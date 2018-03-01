#include "token.h"
#include <stack>

enum TokenKind;

class Stack {
  private:
    std::stack<int> stack;
    void error(std::string msg);

  public:
    void push(int value);
    int pop();
    void clear();
    bool exist();
    void operate(const TokenKind op);
};
