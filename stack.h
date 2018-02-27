#include <stack>

class Stack {
  private:
    std::stack<int> stack;

  public:
    void push(int value);
    int pop();
    void clear();
    bool exist();
};
