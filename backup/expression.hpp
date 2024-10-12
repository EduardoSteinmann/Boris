#ifndef BORIS_EXPRESSION
#define BORIS_EXPRESSION

#include <unordered_map>
#include <vector>
#include <token.hpp>
#include <variant>
#include <type.hpp>

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

struct BinaryOp;
struct LogicalOp;
struct MemberAccess;
struct Dereference;
struct UnaryOp;
struct Grouping;
struct CompoundInitializer;
struct Ternary;
struct Variable;
struct FunctionCall;
struct MethodCall;
struct Literal;

struct Expression
{
    ExpressionType type;
    std::variant<BinaryOp, LogicalOp, MemberAccess, Dereference, UnaryOp, Grouping, CompoundInitializer, Ternary, Variable, FunctionCall, MethodCall, Literal> value;

    template<typename T>
    T& as()
    {
        return std::get<T>(value);
    }
};

struct BinaryOp
{
    Expression left, right;
    Token operation;
};

struct LogicalOp
{
    Expression left, right;
    Token operation;
};

struct MemberAccess
{
    Expression object;
    Token name;
};

struct Dereference
{
    std::vector<Token> dereferences;
    Expression pointer;
};

struct Grouping
{
    Expression expr;
};

struct CompoundInitializer
{
    Type type;
    std::vector<Expression> dot_inits;
};

struct Ternary
{
    Expression if_case;
    Expression else_case;
};

struct Variable
{
    std::string name;
    Type data_type;
};

struct FunctionCall
{
    std::string function_name;
    std::vector<Expression> args;
};

struct MethodCall
{
    Expression object;
    FunctionCall method;
};

struct Literal
{
    Token literal;
};

struct Try
{
    Expression try_expr;
};

#endif