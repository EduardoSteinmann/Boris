#ifndef BORIS_STATEMENT
#define BORIS_STATEMENT

#include "expression.hpp"

enum class StatementType
{
    //MODULE statement exists to have the module as the first statement in the list
    MODULE,
    IMPORT,
    FUNCTION,
    STRUCT,
    ENUM,
    UNION,
    FOR,
    WHILE,
    IF,
    ELIF,
    ELSE,
    INIT,
    ASSIGNMENT,
    PARAM_OR_FIELD,
    MULTIPLE_ASSIGNMENT,
    RETURN,
    DEFER,
    EXPRESSION,
};

struct Statement;

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
    std::variant<Module, Struct, Union, Enum, TaggedUnion, GenericType, Alias, Variable, Function> value;

    template<typename T>
    T& as()
    {
        return std::get<T>(value);
    }
};

using State = std::unordered_map<std::string, Value>;

struct Module
{
    std::string name;
    State globals;
};

struct ImportStatement
{
    std::string module_name;
    std::string module_alias;
};

struct Function
{
    std::string name;
    std::vector<Type> return_type;
    std::vector<Value> parameters;
    std::vector<Value> generics;
    std::vector<Statement> lines;
    State local_state;
};

struct If
{
    //TODO: Maybe change to bool
    std::vector<Expression> condition;
    std::vector<Statement> lines;
    State local_state;
};

struct Else
{
    std::vector<Statement> lines;
    State local_state;
};

struct Struct
{
    std::string name;
    State members;
    State methods;
};

struct Union
{
    std::string name;
    State members;
    State methods;
};

//TODO: write proper enum def
struct Enum
{
    std::vector<std::string> member_names;
};

//TODO: write proper tagged union def
struct TaggedUnion
{
    Union tagged_union;
};

struct For
{
    std::string iter_var;
    Expression iterable;
    std::vector<Statement> lines;
    State local_state;
};

struct While
{
    //TODO: Maybe change to bool
    std::vector<Expression> condition;
    std::vector<Statement> lines;
    State local_state;
};

struct Init
{
    std::string name;
    Expression value;
};

struct Assignment
{
    Expression location;
    Expression value;
};

//TODO: implement
struct MultipleAssignment;

struct Return
{
    std::vector<Expression> return_values;
};

struct Defer
{
    Statement defered_stmt;
};

struct Statement
{
    StatementType type;

    std::variant<Module, ImportStatement, Function, Struct, Enum, Union, Enum, TaggedUnion, For, While, Init, Assignment, MultipleAssignment, Return, Defer> value;

    template<typename T>
    T& as()
    {
        return std::get<T>(value);
    }
};

#endif