#ifndef BORIS_VALUE
#define BORIS_VALUE

#include "token.hpp"
#include <memory>
#include <unordered_map>

enum class ValueType
{
    VARIABLE,
    ALIAS,
    MODULE,
    STRUCT,
    ENUM,
    UNION,
    TAGGED_UNION,
    FUNCTION,
    GENERIC_TYPE,
};

struct Module;
struct Struct;
struct Union;
struct Enum;
struct TaggedUnion;
struct Function;
using GenericType = std::string;
using Alias = std::string;

struct Value
{
    ValueType type;
};

using State = std::unordered_map<std::string, std::unique_ptr<Value>>;

struct Module
{
    Token name;
    State globals;
};

#endif