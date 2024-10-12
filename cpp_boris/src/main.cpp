#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"

void compile(const char* file_name)
{
    auto file = read_file(file_name);
    auto lexer = Lexer(file);
    auto tokens = lexer.tokenize_file();
    auto parser = Parser(file_name, tokens);
    //parser.parse();
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Missing file name";
    }
    compile(argv[1]);
    return 0;
}