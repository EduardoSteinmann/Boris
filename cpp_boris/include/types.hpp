#ifndef TYPES
#define TYPES
#include <common.hpp>

enum class TypeCategory
{
    POINTER,
    STRUCT,
    UNION,
    ENUM,
    NON_COMPLEX,
};

struct Type
{
    TypeCategory category;
    std::string name;
    void* type_information;
};

struct Variable
{
    Type type;
    std::string name;
    CVariable corresponding_c_var;
};

struct CVariable
{
    std::string name;
};

#endif