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
#include <stack>
#include <vector>

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
