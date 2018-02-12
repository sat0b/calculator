#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
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
  bool err;

  void next() { token = tokenizer.nextToken(); }

  void checkKind(TokenKind kind) {
    if (token.getKind() != kind)
      err = true;
  }

  void statement() {
    if (token.getKind() == Variable) {
      std::string name = token.getName();
      next();
      checkKind(Assign);
      if (err)
        return;
      next();
      expression();
      variables[name] = stack.top();
      stack.pop();
    } else if (token.getKind() == Print) {
      next();
      checkKind(Variable);
      if (err)
        return;
      // Get variable name
      std::string name = token.getName();
      // Check format : print a;
      next();
      checkKind(StatementEnd);
      if (err)
        return;
      // Print variable
      std::cout << variables[name] << std::endl;
    } else {
      err = true;
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
      checkKind(RightBracket);
      if (err)
        return;
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
  void run(std::string line) {
    tokenizer.init(line);
    // tokenizer.showTokens();
    err = false;

    while (true) {
      next();
      statement();
      if (err) {
        std::cerr << "Syntax Error " << std::endl;
        return;
      }
      if (token.getKind() == StatementEnd) {
        return;
      } else {
        std::cerr << "Syntax Error" << std::endl;
        return;
      }
    }
  }
};

void repl() {
  Parser parser;
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

void run(std::string fileName) {
  std::ifstream ifs(fileName);
  if (!ifs) {
    std::cerr << "Cannot open " << fileName << std::endl;
    return;
  }
  Parser parser;
  std::string line;
  while (getline(ifs, line)) {
    int n = line.length() - 1;
    if (line[n] != '\n' && line[n] != ';')
      line += ";";
    parser.run(line);
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    std::string fileName = std::string(argv[1]);
    run(fileName);
  }
}
