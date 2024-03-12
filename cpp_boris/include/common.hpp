#ifndef COMMON
#define COMMON
#include <string>

constexpr int SPACES_PER_TAB = 4;
constexpr size_t ASCII_TABLE_SIZE = 128;

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

void debug_print(const char* message);

#endif