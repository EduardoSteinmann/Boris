#ifndef COMMON
#define COMMON
#include <string>
#include <cstdint>

constexpr int SPACES_PER_TAB = 4;
constexpr size_t ASCII_TABLE_SIZE = 128;

#define DEBUG

std::string read_file(std::string file_name);

#ifdef DEBUG
#include <iostream>
#endif

void debug_print(const char* message);

#endif