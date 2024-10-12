#ifndef LEXER
#define LEXER
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include "token.hpp"

class Lexer
{
    public:
    Lexer(const std::string& file_ref);

    std::vector<std::vector<Token>> tokenize_file();

    private:
    const std::string& file;
    std::string::const_iterator file_it;
    std::string::const_iterator begin_parse;
    uint64_t line_num = 1;

    using TokenFunc = Token(Lexer::*)();
    static TokenFunc character_token_funcs[ASCII_TABLE_SIZE];
    static std::unordered_map<std::string, TokenType> keywords;

    Token parse_token();

    char peek();

    Token handle_plus();
    Token handle_minus();
    Token handle_equal();
    Token handle_star();
    Token handle_slash();
    Token handle_right_angle_bracket();
    Token handle_left_angle_bracket();
    Token handle_left_paren();
    Token handle_right_paren();
    Token handle_left_bracket();
    Token handle_right_bracket();
    Token handle_left_brace();
    Token handle_right_brace();
    Token handle_dot();
    Token handle_exclamation_mark();
    Token handle_bitwise_and();
    Token handle_bitwise_not();
    Token handle_bitwise_xor();
    Token handle_bitwise_or();
    Token handle_hash();
    Token handle_carriage_return();
    Token handle_new_line();
    Token handle_spaces();
    Token handle_tab();
    Token handle_colon();
    Token handle_comma();
    Token handle_semi_colon();
    Token handle_char_literal();
    Token handle_str_literal();
    Token handle_number_literals();
};

#endif