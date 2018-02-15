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
  And,
  Or,
  Equal,
  NotEqual,
  LessThan,
  LessEqual,
  GreaterThan,
  GreaterEqual,
  Mod,
  StatementEnd,
  End,
};

std::string getTokenString(TokenKind kind) {
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
  case And:
    return "[And]";
  case Or:
    return "[Or]";
  case Equal:
    return "[Equal]";
  case NotEqual:
    return "[NotEqual]";
  case LessThan:
    return "[LessThan]";
  case LessEqual:
    return "[LessEqual]";
  case GreaterThan:
    return "[GreaterThan]";
  case GreaterEqual:
    return "[GreaterEqual]";
  case Mod:
    return "[Mod]";
  case Print:
    return "[Print]";
  case StatementEnd:
    return "[StatementEnd]";
  case End:
    return "[End]";
  }
}

class Token {
private:
  TokenKind kind;
  std::string token;

  TokenKind getTokenKind(const std::string &token) const {
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
    if (token == "&&")
      return And;
    if (token == "||")
      return Or;
    if (token == "==")
      return Equal;
    if (token == "!=")
      return NotEqual;
    if (token == "<")
      return LessThan;
    if (token == "<=")
      return LessEqual;
    if (token == ">")
      return GreaterThan;
    if (token == ">=")
      return GreaterEqual;
    if (token == "%")
      return Mod;
    if (token == ";")
      return StatementEnd;
    if (token == "print")
      return Print;
    return Variable;
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

  TokenKind getKind() const { return kind; }

  int getValue() const {
    if (std::all_of(token.cbegin(), token.cend(), isdigit)) {
      return std::stoi(token);
    }
    return 0;
  }

  std::string getName() const { return this->token; }

  std::string to_string() {
    std::string ret = getTokenString(kind);
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
  const std::vector<char> operators{'+', '-', '*', '/', '(', ')', '=',
                                    '&', '|', ';', '!', '%', '<', '>'};

  bool checkOperator(char c) const {
    return std::any_of(operators.cbegin(), operators.cend(), [=](int x) {
      if (x == c)
        return true;
      else
        return false;
    });
  }

public:
  void init(const std::string &code) {
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
        // TODO range check code[i + 1]
        if (code[i] == '=') {
          if (code[i + 1] == '=') {
            str = "==";
            i += 2;
          } else {
            str = "=";
            i++;
          }
        } else if (code[i] == '<') {
          if (code[i + 1] == '=') {
            str = "<=";
            i += 2;
          } else {
            str = "<";
            i++;
          }
        } else if (code[i] == '>') {
          if (code[i + 1] == '=') {
            str = ">=";
            i += 2;
          } else {
            str = "<";
            i++;
          }
        } else if (code[i] == '!' && code[i + 1] == '=') {
          str = "!=";
          i += 2;
        } else if (code[i] == '&' || code[i + 1] == '&') {
          str = "&&";
          i += 2;
        } else if (code[i] == '|' || code[i + 1] == '|') {
          str = "||";
          i += 2;
        } else {
          str = code[i];
          i++;
        }
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

  void showTokens() const {
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

  void checkKind(const TokenKind kind) {
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
      orExpression();
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

  void orExpression() {
    andExpression();
    while (token.getKind() == Or) {
      TokenKind op = token.getKind();
      next();
      andExpression();
      operate(op);
    }
  }

  void andExpression() {
    equalExpression();
    while (token.getKind() == And) {
      TokenKind op = token.getKind();
      next();
      equalExpression();
      operate(op);
    }
  }

  void equalExpression() {
    thanExpression();
    while (token.getKind() == Equal || token.getKind() == NotEqual) {
      TokenKind op = token.getKind();
      next();
      thanExpression();
      operate(op);
    }
  }

  void thanExpression() {
    expression();
    while (token.getKind() == LessThan || token.getKind() == LessEqual ||
           token.getKind() == GreaterThan || token.getKind() == GreaterEqual) {
      TokenKind op = token.getKind();
      next();
      expression();
      operate(op);
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
    while (token.getKind() == Product || token.getKind() == Divide ||
           token.getKind() == Mod) {
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

  void operate(const TokenKind op) {
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
    case And:
      stack.push(d1 && d2);
      break;
    case Or:
      stack.push(d1 || d2);
      break;
    case Equal:
      stack.push(d1 == d2);
      break;
    case NotEqual:
      stack.push(d1 != d2);
      break;
    case LessThan:
      stack.push(d1 < d2);
      break;
    case LessEqual:
      stack.push(d1 <= d2);
      break;
    case GreaterThan:
      stack.push(d1 > d2);
      break;
    case GreaterEqual:
      stack.push(d1 >= d2);
      break;
    case Mod:
      stack.push(d1 % d2);
      break;
    default:
      std::cerr << "Not Defined operator : " << getTokenString(op) << std::endl;
      err = 1;
      break;
    }
  }

  void showVariableTable() const {
    for (auto v : variables) {
      std::cout << v.first << " : " << v.second << std::endl;
    }
  }

public:
  void run(const std::string &line) {
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
    const int n = line.length() - 1;
    if (line[n] != '\n' && line[n] != ';')
      line += ";";
    parser.run(line);
    std::cout << ">> ";
  }
}

void run(const std::string &fileName) {
  std::ifstream ifs(fileName);
  if (!ifs) {
    std::cerr << "Cannot open " << fileName << std::endl;
    return;
  }
  Parser parser;
  std::string line;
  while (getline(ifs, line)) {
    const int n = line.length() - 1;
    if (line[n] != '\n' && line[n] != ';')
      line += ";";
    parser.run(line);
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    const std::string fileName = std::string(argv[1]);
    run(fileName);
  }
}
