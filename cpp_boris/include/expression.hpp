#ifndef BORIS_EXPRESSION
#define BORIS_EXPRESSION

#include <unordered_map>
#include <memory>
#include <vector>
#include <token.hpp>
#include <variant>
#include <type.hpp>

#include "statement.hpp"

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

struct BinaryOp: Expression
{
    std::unique_ptr<Expression> left, right;
    Token operation;
};

struct LogicalOp: Expression
{
    std::unique_ptr<Expression> left, right;
    Token operation;
};

struct MemberAccess: Expression
{
    std::unique_ptr<Expression> object;
    Token name;
};

struct Dereference: Expression
{
    std::vector<Token> dereferences;
    std::unique_ptr<Expression> pointer;
};

struct Grouping: Expression
{
    std::unique_ptr<Expression> expr;
};

struct CompoundInitializer: Expression
{
    Type type;
    std::vector<std::unique_ptr<Expression>> dot_inits;
};

struct Ternary: Expression
{
    std::unique_ptr<Expression> if_case;
    std::unique_ptr<Expression> else_case;
};

struct Variable: Expression
{
    Token name;
    Type data_type;
};

struct FunctionCall: Expression
{
    Token function_name;
    std::vector<std::unique_ptr<Expression>> args;
};

struct MethodCall: Expression
{
    std::unique_ptr<Expression> object;
    FunctionCall method;
};

struct Literal: Expression
{
    Token literal;
};

struct Try: Expression
{
    Token try_keyword;
    std::unique_ptr<Expression> try_expr;
};

#endif