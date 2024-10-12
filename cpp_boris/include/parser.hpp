#ifndef BORIS_PARSER
#define BORIS_PARSER
#include <token.hpp>
#include <string>
#include <vector>
#include <common.hpp>
#include <unordered_map>
#include <tuple>
#include "expression.hpp"
#include "statement.hpp"

// class Parser
// {
//     public:
//     Parser(const char* file_name, std::vector<Token>& tokens);
//     Module parse();

//     private:
//     using Line = std::vector<Token>;
//     using LineIterator = std::vector<Token>::const_iterator;
//     using LinesIterator = std::vector<std::vector<Token>>::iterator;
//     using ParseFunc = Statement(Parser::*)();
//     static ParseFunc start_statements[static_cast<uint64_t>(TokenType::NUMBER_OF_TOKEN_TYPES)];

//     //std::vector<Token>& tokens;
//     uint64_t current_indentation_level = 0;
//     Module root;
//     std::vector<Statement> statements;
//     std::vector<Line> lines;
//     LinesIterator file_it;
    
//     std::vector<Line> file_line_by_line(std::vector<Token>& tokens);

//     Module parse_import();
    
//     Type parse_type(LineIterator& line_it);

//     Statement func();
//     Statement import();
//     Statement global_var();
//     Statement struct_def();
//     Statement enum_def();
//     Statement union_def();

//     //TODO: see if func_parameters needs the func parameter or not
//     std::vector<Value> func_parameters(LineIterator& func_it);
//     std::vector<Type> func_return_type(LineIterator& func_it);

//     Statement expr();
//     Statement term();
//     Statement factor();
//     Statement member_access();

//     Statement right_hand_side();
// };

class Parser
{
    public:
    Parser(const char* file_name, std::vector<std::vector<Token>>& tokens);

    std::vector<std::unique_ptr<Statement>> parse_AST();

    private:
    const char* file_name;
    std::vector<std::vector<Token>>& tokens;
    size_t index;
    size_t line_num;

    Token at();
    Token eat();

    //TODO: refactor for errors
    Token expect(TokenType token_type, const char* error_literal);

    bool at_eof();

    std::vector<std::unique_ptr<Statement>> parse_tokens();
    std::vector<std::unique_ptr<Statement>> check_types();
};

#endif