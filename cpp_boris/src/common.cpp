#include "common.hpp"
#include <fstream>

std::string read_file(std::string file_name)
{
    std::ifstream file_stream = std::ifstream(file_name);

    if (!file_stream)
    {
        //TODO: error, file not found
        debug_print("File not found");
    }

    std::string file = "";
    std::string line = "";
    while (std::getline(file_stream, line))
    {
        file += line;
        file += '\n';
    }
    file_stream.close();
    return file;
}

void debug_print(const char* message)
{
    #ifdef DEBUG
    std::cout << message << '\n';
    #endif
}