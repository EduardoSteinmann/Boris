#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"

void compile(const char* file_name)
{
    std::string file = read_file(file_name);
    Lexer lexer = Lexer(file);
    std::vector<Token> tokens = lexer.parse_file();
    Parser parser = Parser(file_name, tokens);
    parser.parse();
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