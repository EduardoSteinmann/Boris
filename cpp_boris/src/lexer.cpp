#include "lexer.hpp"

Lexer::Lexer(const std::string& file_ref): file(file_ref) 
{
    character_token_funcs['+'] = Lexer::handle_plus;
    character_token_funcs['-'] = Lexer::handle_minus;
    character_token_funcs['*'] = Lexer::handle_star;
    character_token_funcs['/'] = Lexer::handle_slash;
    character_token_funcs['<'] = Lexer::handle_left_angle_bracket;
    character_token_funcs['>'] = Lexer::handle_right_angle_bracket;
    character_token_funcs['='] = Lexer::handle_equal;
    character_token_funcs['('] = Lexer::handle_left_paren;
    character_token_funcs[')'] = Lexer::handle_right_paren;
    character_token_funcs['['] = Lexer::handle_left_bracket;
    character_token_funcs[']'] = Lexer::handle_right_bracket;
    character_token_funcs['{'] = Lexer::handle_left_brace;
    character_token_funcs['}'] = Lexer::handle_right_brace;
    character_token_funcs['.'] = Lexer::handle_dot;
    character_token_funcs['!'] = Lexer::handle_exclamation_mark;
    character_token_funcs['&'] = Lexer::handle_bitwise_and;
    character_token_funcs['~'] = Lexer::handle_bitwise_not;
    character_token_funcs['^'] = Lexer::handle_bitwise_xor;
    character_token_funcs['|'] = Lexer::handle_bitwise_or;
    character_token_funcs['#'] = Lexer::handle_hash;
    character_token_funcs['\r'] = Lexer::handle_carriage_return;
    character_token_funcs['\n'] = Lexer::handle_new_line;
    character_token_funcs[' '] = Lexer::handle_spaces;
    character_token_funcs['\t'] = Lexer::handle_tab;
    character_token_funcs[':'] = Lexer::handle_colon;
    character_token_funcs[','] = Lexer::handle_comma;
    character_token_funcs[';'] = Lexer::handle_semi_colon;
    character_token_funcs['\''] = Lexer::handle_char_literal;
    character_token_funcs['"'] = Lexer::handle_str_literal;
}

std::vector<std::vector<Token>> Lexer::tokenize_file()
{
    std::vector<std::vector<Token>> tokens = {};
    std::vector<Token> line = {};
    for (this->file_it = this->file.begin(); this->file_it != this->file.end(); this->file_it++)
    {
        Token token = this->parse_token();

        if (token.token_type == TokenType::IGNORE)
        {
            continue;
        }

        #ifdef DEBUG
        token.print();
        #endif

        line.push_back(token);

        if (token.token_type == TokenType::NEWLINE)
        {
            tokens.push_back(line);
            line.clear();
        }
    }
    return tokens;
}

std::unordered_map<std::string, TokenType> Lexer::keywords = 
{
    { "fn", TokenType::FN },
    { "mut", TokenType::MUT, },
    { "if", TokenType::IF },
    { "elif", TokenType::ELIF },
    { "else", TokenType::ELSE },
    { "while", TokenType::WHILE },
    { "for", TokenType::FOR },
    { "do", TokenType::DO },
    { "import", TokenType::IMPORT },
    { "switch", TokenType::SWITCH },
    { "case", TokenType::CASE },
    { "struct", TokenType::STRUCT },
    { "enum", TokenType::ENUM },
    { "alias", TokenType::ALIAS },
    { "return", TokenType::RETURN },
    { "true", TokenType::TRUE },
    { "false", TokenType::FALSE },
    { "and", TokenType::AND },
    { "or", TokenType::OR },
    { "not", TokenType::NOT },
    { "try", TokenType::TRY },
};

Lexer::TokenFunc Lexer::character_token_funcs[ASCII_TABLE_SIZE] = {};

Token Lexer::parse_token()
{
    this->begin_parse = this->file_it;

    const char current_character = *this->file_it;

    if (character_token_funcs[static_cast<uint64_t>(current_character)])
    {
        return (this->*character_token_funcs[static_cast<uint64_t>(current_character)])();
    }

    if (isdigit(current_character))
    {
        return this->handle_number_literals();
    }

    std::string potential_identifier = "";

    potential_identifier += current_character;

    char next_char = '\0';

    while (isalnum((next_char = this->peek())) || next_char == '_')
    {
        file_it++;
        potential_identifier += *file_it;
    }

    if (potential_identifier.empty())
    {
        //TODO: error
    }

    if (keywords.find(potential_identifier) != keywords.end())
    {
        return Token { keywords[potential_identifier], this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }

    return Token { TokenType::IDENTIFIER, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

char Lexer::peek()
{
    if (this->file_it + 1 == this->file.end())
    {
        return '\0';
    }
    return *(file_it + 1);
}

Token Lexer::handle_plus()
{
    const char next_char = this->peek();
    if (next_char == '=')
    {
        this->file_it++;
        return Token { TokenType::PLUS_EQUAL, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }
    else if (next_char == '+')
    {
        this->file_it++;
        return Token { TokenType::PLUS_PLUS, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }
    return Token { TokenType::PLUS, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_minus()
{
    const char next_char = this->peek();
    if (next_char == '=')
    {
        this->file_it++;
        return Token { TokenType::MINUS, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }
    else if (next_char == '-')
    {
        this->file_it++;
        return Token { TokenType::MINUS_MINUS, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }
    return Token { TokenType::MINUS, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_equal()
{
    const char next_char = this->peek();
    if (next_char == '=')
    {
        this->file_it++;
        return Token { TokenType::EQUAL_EQUAL, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }
    return Token { TokenType::EQUAL, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_star()
{
    const char next_char = this->peek();
    TokenType type = TokenType::STAR;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::STAR_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_slash()
{
    const char next_char = this->peek();
    TokenType type = TokenType::SLASH;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::SLASH_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_right_angle_bracket()
{
    const char next_char = this->peek();
    TokenType type = TokenType::GREATER;
    if (next_char == '>')
    {
        this->file_it++;

        type = TokenType::BITWISE_RIGHT;

        const char next_after_next_char = this->peek();

        if (next_after_next_char == '>')
        {
            file_it++;
            type = TokenType::BITWISE_RIGHT_UNSIGNED;
            const char third_next_char = this->peek();
            if (third_next_char == '=')
            {
                this->file_it++;
                type = TokenType::BITWISE_RIGHT_UNSIGNED_EQUAL;
            }
        }
        else if (next_after_next_char == '=')
        {
            this->file_it++;
            type = TokenType::BITWISE_RIGHT_EQUAL;
        }
    }
    else if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::GREATER_EQUAL;
        return Token { TokenType::GREATER_EQUAL, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }

    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_left_angle_bracket()
{
    const char next_char = this->peek();
    TokenType type = TokenType::LESS;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::LESS_EQUAL;
    }
    else if (next_char == '<')
    {
        this->file_it++;
        type = TokenType::BITWISE_LEFT;
        const char second_next_char = this->peek();
        if (second_next_char == '=')
        {
            type = TokenType::BITWISE_LEFT_EQUAL;
        }
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_left_paren()
{
    return Token { TokenType::LPAREN, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_right_paren()
{
    return Token { TokenType::RPAREN, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_left_bracket()
{
    return Token { TokenType::LBRACKET, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_right_bracket()
{
    return Token { TokenType::RBRACKET, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_left_brace()
{
    return Token { TokenType::LBRACE, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_right_brace()
{
    return Token { TokenType::RBRACE, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_dot()
{
    return Token { TokenType::DOT, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_exclamation_mark()
{
    const char next_char = this->peek();
    TokenType type = TokenType::NOT;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::NOT_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_bitwise_and()
{
    const char next_char = this->peek();
    TokenType type = TokenType::BITWISE_AND;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::BITWISE_AND_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_bitwise_not()
{
    const char next_char = this->peek();
    TokenType type = TokenType::BITWISE_NOT;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::BITWISE_NOT_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_bitwise_xor()
{
    const char next_char = this->peek();
    TokenType type = TokenType::BITWISE_XOR;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::BITWISE_XOR_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_bitwise_or()
{
    const char next_char = this->peek();
    TokenType type = TokenType::BITWISE_OR;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::BITWISE_OR_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

//hashes are comments, so they will be ignored
Token Lexer::handle_hash()
{
    while (this->file_it != file.end() && *this->file_it != '\n')
    {
        this->file_it++;
    }
    return Token { TokenType::IGNORE, this->line_num, "" };
}

Token Lexer::handle_carriage_return()
{
    //TODO: check if - 1 is safe
    if (*(file_it - 1) != '\n')
    {
        return Token { TokenType::IGNORE, this->line_num, "" };
    }
    return Token { TokenType::CARRIAGE_RETURN, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_new_line()
{
    this->line_num++;
    return Token { TokenType::NEWLINE, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_spaces()
{
    //TODO: check if - 1 is safe
    if (*(file_it - 1) != '\n')
    {
        return Token { TokenType::IGNORE, this->line_num, "" };
    }

    while (this->peek() == ' ')
    {
        this->file_it++;
    }
    return Token { TokenType::SPACES, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_tab()
{
    //TODO: check if - 1 is safe
    if (*(file_it - 1) != '\n')
    {
        return Token { TokenType::IGNORE, this->line_num, "" };
    }
    return Token { TokenType::TAB, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_colon()
{
    const char next_char = this->peek();
    TokenType type = TokenType::COLON;
    if (next_char == '=')
    {
        this->file_it++;
        type = TokenType::INIT_EQUAL;
    }
    return Token { type, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_comma()
{
    return Token { TokenType::COMMA, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_semi_colon()
{
    return Token { TokenType::SEMICOLON, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_char_literal()
{
    const char next_char = this->peek();
    this->file_it++;
    if (next_char == '\\')
    {
        this->file_it++;
    }

    const char second_next_char = this->peek();

    if (second_next_char != '\'')
    {
        //TODO: error
    }

    this->file_it++;

    return Token { TokenType::CHAR, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_str_literal()
{
    while (++this->file_it != file.end() && *this->file_it != '"')
    {
        if (*this->file_it == '\n')
        {
            this->line_num++;
        }
        else if (*this->file_it == '"' && *(this->file_it - 1) == '\\')
        {
            this->file_it++;
        }
    }

    if (this->file_it == file.end())
    {
        //TODO: error
    }

    return Token { TokenType::STR, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}

Token Lexer::handle_number_literals()
{
    while (isdigit(this->peek()))
    {
        this->file_it++;
    }

    if (this->peek() == '.')
    {
        file_it++;

        if (!isdigit(this->peek()))
        {
            //TODO: error
        }
        
        file_it++;

        while (isdigit(this->peek()))
        {
            this->file_it++;
        }

        return Token { TokenType::FLOAT, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
    }

    return Token { TokenType::INTEGER, this->line_num, std::string(this->begin_parse, this->file_it + 1) };
}