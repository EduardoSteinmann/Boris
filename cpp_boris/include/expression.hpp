#ifndef EXPRESSION
#define EXPRESSION

#include <unordered_map>
#include <vector>
#include <token.hpp>
#include <variant>

// enum class ExpressionType
// {
//     INTEGER,
//     FLOAT,
//     STRING,
//     ASSIGNMENT,
//     MULTIPLE_ASSIGNMENT,
//     FUNC_CALL,
//     IMPORT,
//     COMPILE_TIME_CONSTANT,
//     METHOD_CALL,
//     IF,
//     ELIF,
//     ELSE,
//     WHILE,
//     DO_WHILE,
//     FOR,
//     FUNC_DEF,
// };

// struct Expression
// {
//     using TokenLine = std::pair<std::vector<Token>::const_iterator, std::vector<Token>::const_iterator>;
//     ExpressionType expression_type;
//     std::vector<Expression> nested_expressions;
//     TokenLine tokens_contained;
// };

// class ExpressionParser
// {
//     public:
//     ExpressionParser(const std::vector<Token>& tokens);
//     std::vector<Expression> parse_expressions();

//     private:
//     using ExpressionParse = Expression(ExpressionParser::*)();
//     ExpressionParse parser_functions[static_cast<size_t>(TokenType::NUMBER_OF_TOKEN_TYPES)];

//     const std::vector<Token>& tokens;
//     const std::vector<Token>::const_iterator current_token;

//     Expression::TokenLine read_next_line_of_tokens();
// };

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
};

enum class ExpressionType
{
    BINARY_OP,
    LOGICAL_OP,
    MEMBER_ACCESS,
    DEREFERENCE,
    UNARY_OP,
    GROUPING,
    COMPOUND_INITIALIZER,
    VARIABLE,
    FUNCTION_CALL,
    METHOD_CALL,
    INTEGER,
    FLOAT,
    STR,
    BOOL,
};

enum class ValueType
{
    VARIABLE,
    MODULE,
    FUNCTION,
    STRUCT,
    ENUM,
    UNION,
};

struct Expression;

struct Statement;

struct Type
{
    uint64_t array_size = 0;
    uint64_t ptr_indirections = 0;
    std::string raw_type_name;
    std::string core_type_name;
    std::vector<std::string> module_localizations;
    std::vector<Type> nested_types;
};

//using Type = std::string;
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
    std::vector<Statement> parameters;
    std::vector<Statement> generics;
    std::vector<Statement> lines;
    State local_state;
};

struct If
{
    //TODO: Maybe change to bool
    std::vector<Statement> condition;
    std::vector<Statement> lines;
    State local_state;
};

struct Else
{
    std::vector<Statement> lines;
    State local_state;
};

struct Variable
{
    std::string name;
    Type data_type;
};

struct Struct
{
    std::string name;
    State members;
    State methods;
};

using StatementValue = std::variant<Variable, Function, Module>;

struct Statement
{
    StatementType type;
    StatementValue value;
};

#endif