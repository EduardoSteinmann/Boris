#ifndef TOKEN
#define TOKEN

#include <string>
#include <cstdint>

enum class TokenType
{
    //Grouping/Braces
    RBRACKET,
    LBRACKET,
    RPAREN,
    LPAREN,
    RBRACE,
    LBRACE,

    //Logical operators
    AND,
    OR,
    NOT,
    NEGATE,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL_EQUAL,

    //Bitwise Operators
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    BITWISE_NOT,
    BITWISE_RIGHT,
    BITWISE_RIGHT_UNSIGNED,
    BITWISE_LEFT,

    //Bitwise compound operators
    BITWISE_AND_EQUAL,
    BITWISE_OR_EQUAL,
    BITWISE_XOR_EQUAL,
    BITWISE_NOT_EQUAL,
    BITWISE_RIGHT_EQUAL,
    BITWISE_RIGHT_UNSIGNED_EQUAL,
    BITWISE_LEFT_EQUAL,

    //Member access
    DOT,
    ARROW,

    //Identifier
    IDENTIFIER,

    //assignment/init
    EQUAL,
    INIT_EQUAL,

    //Arithmetic Operator
    PLUS,
    MINUS,
    STAR,
    SLASH,

    //Arithmetic compound operators
    PLUS_PLUS,
    PLUS_EQUAL,
    SLASH_EQUAL,
    MINUS_EQUAL,
    MINUS_MINUS,
    STAR_EQUAL,

    //line continue
    BACK_SLASH,

    //Keywords
    FN,
    MUT,
    IF,
    ELIF,
    ELSE,
    WHILE,
    FOR,
    DO,
    SWITCH,
    CASE,
    STRUCT,
    ENUM,
    UNION,
    ALIAS,
    TRY,
    RETURN,
    IMPORT,

    //Separating/Grouping
    COLON,
    COMMA,
    SEMICOLON,

    //Whitespace
    SPACES,
    CARRIAGE_RETURN,
    NEWLINE,
    TAB,

    //Comments
    COMMENT,

    //Literals
    INTEGER,
    FLOAT,
    CHAR,
    STR,
    //True and false will be handled as keywords
    TRUE,
    FALSE,

    IGNORE, //Token to ignore being appended

    NUMBER_OF_TOKEN_TYPES
};

struct Token
{
    TokenType token_type;
    uint64_t line_num;
    std::string content;

    #ifdef DEBUG
    void print() const
    {
        static const char* token_type_reprs[static_cast<uint64_t>(TokenType::NUMBER_OF_TOKEN_TYPES)];
        token_type_reprs[static_cast<uint64_t>(TokenType::RBRACKET)] = "RBRACKET";
        token_type_reprs[static_cast<uint64_t>(TokenType::LBRACKET)] = "LBRACKET";
        token_type_reprs[static_cast<uint64_t>(TokenType::RPAREN)] = "RPAREN";
        token_type_reprs[static_cast<uint64_t>(TokenType::LPAREN)] = "LPAREN";
        token_type_reprs[static_cast<uint64_t>(TokenType::RPAREN)] = "RPAREN";
        token_type_reprs[static_cast<uint64_t>(TokenType::RBRACE)] = "RBRACE";
        token_type_reprs[static_cast<uint64_t>(TokenType::LBRACE)] = "LBRACE";
        token_type_reprs[static_cast<uint64_t>(TokenType::AND)] = "AND";
        token_type_reprs[static_cast<uint64_t>(TokenType::OR)] = "OR";
        token_type_reprs[static_cast<uint64_t>(TokenType::NOT)] = "NOT";
        token_type_reprs[static_cast<uint64_t>(TokenType::NEGATE)] = "NEGATE";
        token_type_reprs[static_cast<uint64_t>(TokenType::NOT_EQUAL)] = "NOT_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::GREATER)] = "GREATER";
        token_type_reprs[static_cast<uint64_t>(TokenType::LESS)] = "LESS";
        token_type_reprs[static_cast<uint64_t>(TokenType::GREATER_EQUAL)] = "GREATER_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::LESS_EQUAL)] = "LESS_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::EQUAL_EQUAL)] = "EQUAL_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_AND)] = "BITWISE_AND";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_OR)] = "BITWISE_OR";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_XOR)] = "BITWISE_XOR";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_NOT)] = "BITWISE_NOT";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_RIGHT)] = "BITWISE_RIGHT";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_RIGHT_UNSIGNED)] = "BITWISE_RIGHT_UNSIGNED";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_LEFT)] = "BITWISE_LEFT";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_AND_EQUAL)] = "BITWISE_AND_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_OR_EQUAL)] = "BITWISE_OR_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_XOR_EQUAL)] = "BITWISE_XOR_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_NOT_EQUAL)] = "BITWISE_NOT_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_RIGHT_EQUAL)] = "BITWISE_RIGHT_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_RIGHT_UNSIGNED_EQUAL)] = "BITWISE_RIGHT_UNSIGNED_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BITWISE_LEFT_EQUAL)] = "BITWISE_LEFT_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::DOT)] = "DOT";
        token_type_reprs[static_cast<uint64_t>(TokenType::ARROW)] = "ARROW";
        token_type_reprs[static_cast<uint64_t>(TokenType::IDENTIFIER)] = "IDENTIFIER";
        token_type_reprs[static_cast<uint64_t>(TokenType::EQUAL)] = "EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::INIT_EQUAL)] = "INIT_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::PLUS)] = "PLUS";
        token_type_reprs[static_cast<uint64_t>(TokenType::MINUS)] = "MINUS";
        token_type_reprs[static_cast<uint64_t>(TokenType::STAR)] = "STAR";
        token_type_reprs[static_cast<uint64_t>(TokenType::SLASH)] = "SLASH";
        token_type_reprs[static_cast<uint64_t>(TokenType::PLUS_PLUS)] = "PLUS_PLUS";
        token_type_reprs[static_cast<uint64_t>(TokenType::MINUS_MINUS)] = "MINUS_MINUS";
        token_type_reprs[static_cast<uint64_t>(TokenType::MINUS_EQUAL)] = "MINUS_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::PLUS_EQUAL)] = "PLUS_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::SLASH_EQUAL)] = "SLASH_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::STAR_EQUAL)] = "STAR_EQUAL";
        token_type_reprs[static_cast<uint64_t>(TokenType::BACK_SLASH)] = "BACK_SLASH";
        token_type_reprs[static_cast<uint64_t>(TokenType::FN)] = "FN";
        token_type_reprs[static_cast<uint64_t>(TokenType::MUT)] = "MUT";
        token_type_reprs[static_cast<uint64_t>(TokenType::IF)] = "IF";
        token_type_reprs[static_cast<uint64_t>(TokenType::ELIF)] = "ELIF";
        token_type_reprs[static_cast<uint64_t>(TokenType::WHILE)] = "WHILE";
        token_type_reprs[static_cast<uint64_t>(TokenType::FOR)] = "FOR";
        token_type_reprs[static_cast<uint64_t>(TokenType::DO)] = "DO";
        token_type_reprs[static_cast<uint64_t>(TokenType::SWITCH)] = "SWITCH";
        token_type_reprs[static_cast<uint64_t>(TokenType::CASE)] = "CASE";
        token_type_reprs[static_cast<uint64_t>(TokenType::STRUCT)] = "STRUCT";
        token_type_reprs[static_cast<uint64_t>(TokenType::ENUM)] = "ENUM";
        token_type_reprs[static_cast<uint64_t>(TokenType::UNION)] = "UNION";
        token_type_reprs[static_cast<uint64_t>(TokenType::ALIAS)] = "ALIAS";
        token_type_reprs[static_cast<uint64_t>(TokenType::RETURN)] = "RETURN";
        token_type_reprs[static_cast<uint64_t>(TokenType::IMPORT)] = "IMPORT";
        token_type_reprs[static_cast<uint64_t>(TokenType::COLON)] = "COLON";
        token_type_reprs[static_cast<uint64_t>(TokenType::COMMA)] = "COMMA";
        token_type_reprs[static_cast<uint64_t>(TokenType::SEMICOLON)] = "SEMICOLON";
        token_type_reprs[static_cast<uint64_t>(TokenType::SPACES)] = "SPACES";
        token_type_reprs[static_cast<uint64_t>(TokenType::CARRIAGE_RETURN)] = "CARRIAGE_RETURN";
        token_type_reprs[static_cast<uint64_t>(TokenType::NEWLINE)] = "NEWLINE";
        token_type_reprs[static_cast<uint64_t>(TokenType::TAB)] = "TAB";
        token_type_reprs[static_cast<uint64_t>(TokenType::COMMENT)] = "COMMENT";
        token_type_reprs[static_cast<uint64_t>(TokenType::INTEGER)] = "INTEGER";
        token_type_reprs[static_cast<uint64_t>(TokenType::FLOAT)] = "FLOAT";
        token_type_reprs[static_cast<uint64_t>(TokenType::CHAR)] = "CHAR";
        token_type_reprs[static_cast<uint64_t>(TokenType::STR)] = "STR";
        token_type_reprs[static_cast<uint64_t>(TokenType::TRUE)] = "TRUE";
        token_type_reprs[static_cast<uint64_t>(TokenType::FALSE)] = "FALSE";
        token_type_reprs[static_cast<uint64_t>(TokenType::TRY)] = "TRY";
        std::cout << "Token { token_type = " << token_type_reprs[static_cast<uint64_t>(this->token_type)] << ", line_num = " << this->line_num << ", content = " << this->content << " }\n";
    }
    #endif
};

#endif