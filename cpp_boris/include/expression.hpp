#ifndef EXPRESSION
#define EXPRESSION

#include <vector>
#include <token.hpp>

enum class ExpressionType
{
    INTEGER,
    FLOAT,
    STRING,
    ASSIGNMENT,
    MULTIPLE_ASSIGNMENT,
    FUNC_CALL,
    IMPORT,
    COMPILE_TIME_CONSTANT,
    METHOD_CALL,
    IF,
    ELIF,
    ELSE,
    WHILE,
    DO_WHILE,
    FOR,
    FUNC_DEF,
};


struct Expression
{
    using TokenLine = std::pair<std::vector<Token>::const_iterator, std::vector<Token>::const_iterator>;
    ExpressionType expression_type;
    std::vector<Expression> nested_expressions;
    TokenLine tokens_contained;
};

class ExpressionParser
{
    public:
    ExpressionParser(const std::vector<Token>& tokens);
    std::vector<Expression> parse_expressions();

    private:
    const std::vector<Token>& tokens;

    Expression::TokenLine read_next_line_of_tokens();
};

#endif