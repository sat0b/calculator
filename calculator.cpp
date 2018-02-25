#include "parser.h"
#include "token.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <vector>

// void repl() {
//   bool replMode = true;
//   Parser parser;
//   std::string line;
//   std::cout << ">> ";
//   while (getline(std::cin, line)) {
//     const int n = line.length() - 1;
//     if (line[n] != '\n' && line[n] != ';')
//       line += ";";
//     parser.run(line, replMode);
//     std::cout << ">> ";
//   }
// }

void run(const std::string &fileName) {
  bool replMode = false;
  std::ifstream ifs(fileName);
  if (!ifs) {
    std::cerr << "Cannot open " << fileName << std::endl;
    return;
  }
  std::string line;
  std::string code;
  while (getline(ifs, line)) {
    const int n = line.length() - 1;
    if (line[n] != '\n' && line[n] != ';')
      line += ";";
    code += line;
  }
  Lexer *lexer = new Lexer(code);
  Parser parser(lexer);
  parser.run();
}

int main(int argc, char **argv) {
  if (argc == 1) {
    // repl();
  } else if (argc == 2) {
    const std::string fileName = std::string(argv[1]);
    run(fileName);
  }
}
