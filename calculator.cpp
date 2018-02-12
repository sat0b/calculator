#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <stack>
#include <vector>

enum TokenKind {
  Integer,
  Product,
  Plus,
  Divide,
  Minus,
  Symbol,
  LeftBracket,
  RightBracket,
  Variable,
  Assign,
  Print,
  StatementEnd,
  End,
};

class Token {
private:
  TokenKind kind;
  std::string token;

  TokenKind getTokenKind(std::string token) {
    if (all_of(token.cbegin(), token.cend(), isdigit))
      return Integer;
    if (token == "+")
      return Plus;
    if (token == "-")
      return Minus;
    if (token == "*")
      return Product;
    if (token == "/")
      return Divide;
    if (token == "(")
      return LeftBracket;
    if (token == ")")
      return RightBracket;
    if (token == "=")
      return Assign;
    if (token == ";")
      return StatementEnd;
    if (token == "print")
      return Print;
    return Variable;
  }

  std::string getSymbolString() {
    switch (kind) {
    case Integer:
      return "[Integer]";
    case Product:
      return "[Product]";
    case Plus:
      return "[Plus]";
    case Divide:
      return "[Divide]";
    case Minus:
      return "[Minus]";
    case Symbol:
      return "[Symbol]";
    case LeftBracket:
      return "[LeftBracket]";
    case RightBracket:
      return "[RightBracket]";
    case Variable:
      return "[Variable]";
    case Assign:
      return "[Assign]";
    case Print:
      return "[Print]";
    case StatementEnd:
      return "[StatementEnd]";
    case End:
      return "[End]";
    }
  }

public:
  Token() {}

  Token(std::string str) {
    this->kind = getTokenKind(str);
    this->token = str;
  }

  Token(TokenKind kind) {
    this->kind = kind;
    this->token = "";
  }

  TokenKind getKind() { return kind; }

  int getValue() {
    if (std::all_of(token.cbegin(), token.cend(), isdigit)) {
      return std::stoi(token);
    }
    return 0;
  }

  std::string getName() { return this->token; }

  std::string to_string() {
    std::string ret = getSymbolString();
    return ret + " " + token;
  }

  static Token getEndToken() {
    Token token(End);
    return token;
  }
};

class Tokenizer {
private:
  std::string code;
  std::vector<Token> tokens;
  std::vector<Token>::iterator itr;
  std::vector<Token>::iterator iend;
  const std::vector<char> operators{'+', '-', '*', '/', '(', ')', '=', ';'};

  bool checkOperator(char c) {
    return std::any_of(operators.cbegin(), operators.cend(), [=](int x) {
      if (x == c)
        return true;
      else
        return false;
    });
  }

public:
  void init(std::string code) {
    tokens.clear();
    int i = 0;
    while (i < code.length()) {
      if (code[i] == ' ') {
        i++;
        continue;
      }

      std::string str;
      // digit
      if (isdigit(code[i])) {
        while (isdigit(code[i])) {
          str += code[i];
          i++;
        }
      }
      // operator
      else if (checkOperator(code[i])) {
        str = code[i];
        i++;
      }
      // variable
      else {
        while (code[i] != ' ' && !checkOperator(code[i])) {
          str += code[i];
          i++;
        }
      }
      Token token(str);
      tokens.push_back(token);
    }
    Token token(End);
    tokens.push_back(token);
    itr = tokens.begin();
    iend = tokens.end();
  };

  Token nextToken() {
    if (itr != iend)
      return *itr++;
    return Token::getEndToken();
  }

  void showTokens() {
    for (auto t : tokens) {
      std::cout << t.to_string() << std::endl;
    }
  }
};

class Parser {
private:
  Tokenizer tokenizer;
  Token token;
  std::stack<int> stack;
  std::map<std::string, int> variables;

  void next() { token = tokenizer.nextToken(); }

  bool checkKind(TokenKind kind) { return token.getKind() == kind; }

  bool statement() {
    if (token.getKind() == Variable) {
      std::string name = token.getName();
      next();
      if (!checkKind(Assign))
        return false;
      next();
      expression();
      variables[name] = stack.top();
      stack.pop();
      return true;
    } else if (token.getKind() == Print) {
      next();
      if (!checkKind(Variable))
        return false;
      std::string name = token.getName();
      std::cout << variables[name] << std::endl;
      next();
      return true;
    } else {
      return false;
    }
  }

  void expression() {
    term();
    while (token.getKind() == Plus || token.getKind() == Minus) {
      TokenKind op = token.getKind();
      next();
      term();
      operate(op);
    }
  }

  void term() {
    factor();
    while (token.getKind() == Product || token.getKind() == Divide) {
      TokenKind op = token.getKind();
      next();
      factor();
      operate(op);
    }
  }

  void factor() {
    switch (token.getKind()) {
    case Integer:
      stack.push(token.getValue());
      break;
    case Variable:
      stack.push(variables[token.getName()]);
      break;
    case LeftBracket:
      next();
      expression();
      assert(token.getKind() == RightBracket);
      break;
    default:
      break;
    }
    token = tokenizer.nextToken();
  }

  void operate(TokenKind op) {
    int d2 = stack.top();
    stack.pop();
    int d1 = stack.top();
    stack.pop();

    switch (op) {
    case Plus:
      stack.push(d1 + d2);
      break;
    case Minus:
      stack.push(d1 - d2);
      break;
    case Product:
      stack.push(d1 * d2);
      break;
    case Divide:
      stack.push(d1 / d2);
      break;
    default:
      break;
    }
  }

  void showVariableTable() {
    for (auto v : variables) {
      std::cout << v.first << " : " << v.second << std::endl;
    }
  }

public:
  bool run(std::string line) {
    tokenizer.init(line);

    while (true) {
      // showVariableTable();
      next();
      if (token.getKind() == End) {
        break;
      }
      if (!statement()) {
        std::cerr << "Syntax Error" << std::endl;
        next(); // for StatementEnd
        continue;
      }
      assert(token.getKind() == StatementEnd || token.getKind() == End);
      if (token.getKind() == End) {
        break;
      }
    }
    while (stack.size() > 0) {
      std::cout << stack.top() << std::endl;
      stack.pop();
    }
    return true;
  }
};

int main(int argc, char **argv) {
  Parser parser;

  // iterpreter mode
  std::string line;
  std::cout << ">> ";
  while (getline(std::cin, line)) {
    int n = line.length() - 1;
    if (line[n] != '\n' && line[n] != ';')
      line += ";";
    parser.run(line);
    std::cout << ">> ";
  }
}
