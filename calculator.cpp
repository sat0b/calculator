#include "ast.h"
#include "parser.h"
#include "runner.h"
#include "token.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

void run(const std::string &file_name) {
    std::ifstream ifs(file_name);
    if (!ifs) {
        std::cerr << "Cannot open " << file_name << std::endl;
        return;
    }
    std::string line;
    std::string code;
    while (getline(ifs, line)) {
        const size_t n = line.length() - 1;
        if (line[n] != '\n' && line[n] != ';')
            line += ";";
        code += line;
    }
    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(code);
    Parser parser(std::move(lexer));
    std::vector<Ast *> astvec = parser.parse();
    Runner runner(astvec);
    runner.run();
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // repl();
    } else if (argc == 2) {
        const std::string file_name = std::string(argv[1]);
        run(file_name);
    }
}
