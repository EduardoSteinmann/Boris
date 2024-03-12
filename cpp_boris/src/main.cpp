#include <fstream>
#include "lexer.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

std::string read_file()
{
    std::ifstream test_file = std::ifstream("test.boris");
    std::string file = "";
    std::string line = "";
    while (std::getline(test_file, line))
    {
        file += line;
        file += '\n';
    }
    test_file.close();
    return file;
}

void compile()
{
    std::string file = read_file();
    Lexer lexer = Lexer(file);
    lexer.parse_file();
}

int main()
{
    compile();
    return 0;
}