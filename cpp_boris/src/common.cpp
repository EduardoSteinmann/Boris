#include "common.hpp"

void debug_print(const char* message)
{
    #ifdef DEBUG
    std::cout << message << '\n';
    #endif
}