#ifndef BORIS_TYPE
#define BORIS_TYPE

#include <cinttypes>
#include <vector>
#include <string>
#include <unordered_map>

struct Type
{
    uint64_t array_size = 0;
    uint64_t ptr_indirections = 0;
    std::string raw_type_name;
    std::string core_type_name;
    //std::unordered_map<std::string, Type> generics;
    std::vector<std::string> module_localizations;
    std::vector<Type> nested_types;
};

#endif