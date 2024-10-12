#ifndef BORIS_STATEMENT
#define BORIS_STATEMENT

#include "value.hpp"
#include "type.hpp"

enum class ExpressionType
{
    BINARY_OP,
    LOGICAL_OP,
    MEMBER_ACCESS,
    DEREFERENCE,
    UNARY_OP,
    GROUPING,
    COMPOUND_INITIALIZER,
    TERNARY,
    VARIABLE,
    FUNCTION_CALL,
    METHOD_CALL,
    TRY,
    INTEGER,
    FLOAT,
    STR,
    BOOL,
};

struct Expression
{
    Type eval_type;
    ExpressionType expr_type;
};

enum class StatementType
{
    //MODULE statement exists to have the module as the first statement in the list
    MODULE,
    IMPORT,
    FUNCTION,
    STRUCT,
    MEMBER_INITIALIZER,
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
};

struct Statement
{
    StatementType stmt_type;
};

struct ImportStatement: Statement
{
    Token module_name;
    Token module_alias;
};

struct Function: Statement
{
    Token name;
    std::vector<Type> return_types;
    std::vector<Variable> parameters;
    std::vector<Type> generics;
    std::vector<Statement> lines;
    State local_state;
};

struct If: Statement
{
    //TODO: Maybe change to bool
    std::unique_ptr<Expression> condition;
    std::vector<Statement> lines;
    State local_state;
};

struct Else: Statement
{
    std::vector<Statement> lines;
    State local_state;
};

struct Struct: Statement
{
    Token name;
    State members;
    State methods;
};

struct MemberInitializer: Statement
{
    Token member_name;
    std::unique_ptr<Expression> value;
};

struct Union: Statement
{
    Token name;
    State members;
    State methods;
};

//TODO: write proper enum def
struct Enum: Statement
{
    std::vector<Token> member_names;
};

//TODO: write proper tagged union def
struct TaggedUnion: Statement
{
    Union tagged_union;
};

struct For: Statement
{
    Token iter_var;
    std::unique_ptr<Expression> iterable;
    std::vector<Statement> lines;
    State local_state;
};

struct While: Statement
{
    //TODO: Maybe change to bool
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> lines;
    State local_state;
};

struct Init: Statement
{
    Variable name;
    std::unique_ptr<Expression> value;
};

struct Assignment: Statement
{
    Variable name;
    std::unique_ptr<Expression> value;
};

//TODO: implement
struct MultipleAssignment;

struct Return: Statement
{
    std::vector<std::unique_ptr<Expression>> return_values;
};

struct Defer: Statement
{
    std::unique_ptr<Statement> defered_stmt;
};

#endif