#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <algorithm>

constexpr const char* BORIS_VERSION = "0.1.0";

enum class TokenType
{
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_PAREN, RIGHT_PAREN,
    INIT_EQUAL,
    FN,
    ELSE, IF, ELIF, 
    STATIC, EXTERN, TYPEDEF,
    FOR, WHILE,
    ENUM, UNION, STRUCT,
    PREPROCESSOR_DIRECTIVE,
    INCLUDE, IMPORT,
    OTHER,
    EOL
};

struct Token
{
    TokenType type;
    std::string content;
    size_t start;
    size_t end;
};

class Lexer
{
    public:
    //Prefer pointer because syntax is more explicit than reference
    static Lexer create(const std::string* source)
    {
        Lexer instance;
        instance.key_words = 
        {
            { "else", TokenType::ELSE },
            { "if", TokenType::IF },
            { "for", TokenType::FOR },
            { "fn", TokenType::FN },
            { "while", TokenType::WHILE },
            { "import", TokenType::IMPORT },
            { "static", TokenType::STATIC },
            { "extern", TokenType::EXTERN },
            { "typedef", TokenType::TYPEDEF },
            { "struct", TokenType::STRUCT },
            { "enum", TokenType::ENUM },
            { "union", TokenType::UNION },
        };

        instance.source = source;
        return instance;
    }

    std::vector<Token> parse_tokens()
    {
        while (!reached_end())
        {
            start = current;
            parse_token();
        }
        return tokens;
    }

    private:
    std::unordered_map<std::string, TokenType> key_words;
    std::vector<Token> tokens;
    const std::string* source;
    size_t start = 0;
    size_t current = 0;

    void parse_token()
    {
        const char to_examine = next();

        switch (to_examine)
        {
            case '{': add_token(TokenType::LEFT_BRACE); break;
            case '}': add_token(TokenType::RIGHT_BRACE); break;
            case '(': add_token(TokenType::LEFT_PAREN); break;
            case ')': add_token(TokenType::RIGHT_PAREN); break;
            case ':': 
                if (compare_next_and_advance_if_true('='))
                {
                    add_token(TokenType::INIT_EQUAL);
                } 
                else 
                {
                    add_token(TokenType::OTHER); 
                } 
                break;
            case '#': 
                while (peek_next() != '\n' && !reached_end()) next();
                add_token(TokenType::PREPROCESSOR_DIRECTIVE);
                break;
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                break;
            default:
                if (is_alpha(to_examine)) 
                {
                    add_alpha_numeric_other();
                }
                else
                {
                    add_token(TokenType::OTHER);
                }
                break;
        }
    }

    void add_alpha_numeric_other()
    {
        while (is_alphanumeric(peek_next()))
        {
            next();
        }

        TokenType type = TokenType::LEFT_BRACE;

        std::string text = source->substr(start, current - start);
        type = key_words[text];

        if (type == TokenType::LEFT_BRACE)
        {
            type = TokenType::OTHER;
        }

        add_token(type);
    }
    
    const bool is_alphanumeric(const char character)
    {
        return is_alpha(character) || is_digit(character);
    }

    const bool is_alpha(const char character)
    {
        return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || character == '_';
    }

    const bool is_digit(const char character)
    {
        return character >= '0' && character <= '9';
    }

    const char get_char(const int index)
    {
        return (*source)[index];
    }

    const char peek_next()
    {
        return reached_end() ? '\0' : get_char(current);
    }

    const char peek_twice()
    {
        return (current + 1) >= source->length() ? '\0' : get_char(current + 1);
    }

    const bool compare_next_and_advance_if_true(const char to_compare_against)
    {
        if (reached_end() || get_char(current) != to_compare_against)
        {
            return false;
        }

        current++;
        return true;
    }

    const char next()
    {
        current++;
        return get_char(current - 1);
    }

    const bool reached_end()
    {
        return current >= source->length();
    }

    void add_token(TokenType token_type)
    {
        std::string content = source->substr(start, current - start);
        tokens.push_back( Token { token_type, content, start, current } );
    }
}; 

struct ForwardDeclrs
{
    std::vector<std::string> preprocessor_declrs, type_declrs, struct_declrs, function_declrs, global_declrs;
};

int translate_file(const std::string& file_name);

const std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
    const size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";

    const size_t strEnd = str.find_last_not_of(whitespace);
    const size_t strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

const std::string to_upper_case(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string generate_line(const std::vector<Token>& tokens, const std::string& line, const std::string trimmed_line, ForwardDeclrs& forward_declrs, bool& in_global_scope, size_t& braces_count)
{
    if (tokens.size() == 0)
    {
        return "";
    }

    static std::unordered_map<std::string, bool> imported_files = {};
    static size_t num_of_structs = 0;
    static size_t open_struct_braces = 0;
    static size_t close_struct_braces = 0;

    size_t prev_close_struct_braces = close_struct_braces;

    static bool parsing_enum = false;

    bool need_semi = true;

    if (parsing_enum)
    {
        need_semi = false;
    }
    
    const Token& first_token = tokens[0];

    std::string c_line = line;

    switch (first_token.type)
    {
        case TokenType::FN:
        {
            need_semi = false;

            if (in_global_scope)
            {
                in_global_scope = false;
            }

            bool static_type = false;
            std::string type_name = "";
            //use a for loop in case of it not finding parens, so it terminates definitely
            for (size_t i = tokens.size() - 1; i >= 0; i--)
            {
                const Token& token = tokens[i];
                if (token.type == TokenType::RIGHT_PAREN)
                {
                    break;
                }
                else if (token.type == TokenType::STATIC)
                {
                    static_type = true;
                }
                type_name += token.content + ' ';
            }

            for (size_t i = line.length() - 1; i >= 0; i--)
            {
                if (line[i] == ')')
                {
                    break;
                }
                c_line[i] = ' ';
            }

            c_line[0] = ' ';
            c_line[1] = ' ';
            c_line.insert(first_token.start, type_name);
            if (!static_type)
            {
                forward_declrs.function_declrs.push_back(c_line + ";");
            }
            break;
        }
        case TokenType::IF:
        {
            need_semi = false;
            size_t closing_paren_pos = tokens.size() - 1;
            if (tokens[closing_paren_pos].type == TokenType::LEFT_BRACE)
            {
                closing_paren_pos--;
            }
            c_line.insert(first_token.end, " (");
            c_line.insert(tokens[closing_paren_pos].end + 2, " )"); //+ 2 to offset the insertion of " ("
            break;
        }
        case TokenType::ELSE:
        {
            need_semi = false;
            size_t closing_paren_pos = tokens.size() - 1;
            if (tokens.size() < 2)
            {
                break;
            }
            else if (tokens[closing_paren_pos].type == TokenType::LEFT_BRACE)
            {
                closing_paren_pos--;
            }
            if (tokens[1].type == TokenType::IF)
            {
                c_line.insert(tokens[1].end, " (");
                c_line.insert(tokens[closing_paren_pos].end + 2, " )");
            }
            break;
        }
        case TokenType::WHILE:
        {
            need_semi = false;
            size_t closing_paren_pos = tokens.size() - 1;
            if (tokens[closing_paren_pos].type == TokenType::LEFT_BRACE)
            {
                closing_paren_pos--;
            }
            c_line.insert(first_token.end, " (");
            c_line.insert(tokens[closing_paren_pos].end + 2, " )");
            break;
        }
        case TokenType::FOR:
        {
            need_semi = false;
            size_t closing_paren_pos = tokens.size() - 1;
            if (tokens[closing_paren_pos].type == TokenType::LEFT_BRACE)
            {
                closing_paren_pos--;
            }
            c_line.insert(first_token.end, " (");
            c_line.insert(tokens[closing_paren_pos].end + 2, " )");
            break;
        }
        case TokenType::EXTERN:
        {
            if (in_global_scope)
            {
                const size_t equal_pos = c_line.find('=');

                if (equal_pos != std::string::npos)
                {
                    const std::string assignment = trimmed_line.substr(6, trimmed_line.length() - 6) + ';';
                    const std::string declaration = trimmed_line.substr(0, equal_pos) + ';';
                    
                    forward_declrs.global_declrs.push_back(declaration);
                    return assignment;
                } 
            }
            return "";
        }
        case TokenType::PREPROCESSOR_DIRECTIVE:
        {
            need_semi = false;
            if (in_global_scope)
            {
                forward_declrs.preprocessor_declrs.push_back(first_token.content);
                return "";
            }
            break;
        }
        case TokenType::RIGHT_BRACE:
        {
            if (tokens.size() == 1)
            {
                if (!in_global_scope && !(--braces_count))
                {
                    in_global_scope = true;
                }
                if (num_of_structs)
                {
                    close_struct_braces++;
                    c_line += ';';
                    if (open_struct_braces == close_struct_braces)
                    {
                        num_of_structs = 0;
                        open_struct_braces = 0;
                        close_struct_braces = 0;
                        if (parsing_enum)
                        {
                            parsing_enum = false;
                        }
                    }
                    forward_declrs.struct_declrs.push_back(c_line);
                    return "";
                }
                return c_line;
            }
            break;
        }
        case TokenType::LEFT_BRACE:
        {
            if (tokens.size() == 1)
            {
                if (!in_global_scope)
                {
                    ++braces_count;
                }
                if (num_of_structs)
                {
                    open_struct_braces++;
                    forward_declrs.struct_declrs.push_back(c_line);
                    return "";
                }
                return c_line;
            }
            break;
        }
        case TokenType::TYPEDEF:
        {
            if (in_global_scope)
            {
                forward_declrs.type_declrs.push_back(c_line + ";");
                return "";
            }
            break;
        }
        case TokenType::ENUM: parsing_enum = true;
        case TokenType::UNION:
        case TokenType::STRUCT:
        {
            num_of_structs++;
            need_semi = false;
            std::string typedef_declr = "";
            if (tokens.size() > 1 && tokens[1].type == TokenType::OTHER)
            {
                typedef_declr = "typedef " + trimmed_line + ' ' + tokens[1].content + ';';
                forward_declrs.type_declrs.push_back(typedef_declr);
            }
            forward_declrs.struct_declrs.push_back(c_line);
            return "";
            break;
        }
        case TokenType::IMPORT:
        {
            need_semi = false;
            std::string file_to_import = "";
            for (size_t i = 0; i < tokens.size(); i++)
            {
                const Token& token = tokens[i];
                if (token.type == TokenType::IMPORT)
                {
                    continue;
                }
                else
                {
                    file_to_import += token.content;
                }
            }
            if (!imported_files[file_to_import])
            {
                imported_files[file_to_import] = true;
                translate_file(file_to_import);
                forward_declrs.preprocessor_declrs.push_back("#include \"" + file_to_import + ".h\"");
                return "";
            }
            return "";
            break;
        }
    }

    size_t init_equal_pos = 0;

    for (const Token& token : tokens)
    {
        if (token.type == TokenType::LEFT_BRACE && !in_global_scope)
        {
            if (num_of_structs)
            {
                open_struct_braces++;
            }
            ++braces_count;
        }
        else if (token.type == TokenType::RIGHT_BRACE && !in_global_scope)
        {
            if (num_of_structs)
            {
                ++close_struct_braces;
            }
            if (!(--braces_count))
            {
                in_global_scope = true;
            }
        }
    }

    if (num_of_structs)
    {
        if (close_struct_braces > prev_close_struct_braces)
        {
            c_line += ';';
            need_semi = false;
        }
        else if (open_struct_braces == close_struct_braces)
        {
            num_of_structs = 0;
            open_struct_braces = 0;
            close_struct_braces = 0;
            if (parsing_enum)
            {
                parsing_enum = false;
            }
            c_line += ';';
            forward_declrs.struct_declrs.push_back(c_line);
            return "";
        }
    }

    for (size_t i = 0; i < tokens.size(); i++)
    {
        const Token& token = tokens[i];
        if (token.type == TokenType::INIT_EQUAL)
        {
            init_equal_pos = i;
        }
    }

    if (init_equal_pos)
    {
        const char* type_deduction = "const auto ";

        init_equal_pos = tokens[init_equal_pos].start;

        for (size_t i = init_equal_pos - 1; i >= 0; i--)
        {
            if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r')
            {
                continue;
            }

            if (init_equal_pos - 3 > 0)
            {
                const std::string& mut_keyword = trim(line.substr(init_equal_pos - 4, 3));
                if (mut_keyword == "mut")
                {
                    type_deduction = "auto ";
                    const size_t mut_pos = init_equal_pos - 4;
                    for (size_t i = 0; i < 4; i++)
                    {
                        c_line[i + mut_pos] = ' ';
                    }
                }
            }
            break;
        }

        c_line[init_equal_pos] = ' ';

        c_line.insert(first_token.start, type_deduction);
    }

    if (need_semi)
    {
        c_line += ';';
    }

    if (num_of_structs)
    {
        forward_declrs.struct_declrs.push_back(c_line);
        return "";
    }

    return c_line;
}

int translate_file(const std::string& file_name)
{
    std::ifstream src_file = std::ifstream(file_name);
    //std::filesystem::create_directory("boris_files");
    std::ofstream c_file = std::ofstream(file_name + ".c");
    std::ofstream h_file = std::ofstream(file_name + ".h");

    std::string line = "";
    ForwardDeclrs forward_declrs = {};

    c_file << "#include \"" + file_name + ".h\"\n";

    bool in_global_scope = true;
    size_t braces_count = 0;

    while (std::getline(src_file, line))
    {
        const std::string trimmed_line = trim(line);
        Lexer lexer = Lexer::create(&line);
        const std::vector<Token> tokens = lexer.parse_tokens();
        c_file << generate_line(tokens, line, trimmed_line, forward_declrs, in_global_scope, braces_count) << '\n';
    }

    std::string header_guard = to_upper_case(file_name);
    header_guard.erase(std::remove_if(header_guard.begin(), header_guard.end(), [](const char character){ return !std::isalnum(character); }), header_guard.end());
    header_guard += "_H";

    h_file << "#ifndef " << header_guard << '\n';
    h_file << "#define " << header_guard << '\n';

    for (const std::string& preprocessor_declr : forward_declrs.preprocessor_declrs)
    {
        h_file << preprocessor_declr << '\n';
    }

    for (const std::string& type_declr : forward_declrs.type_declrs)
    {
        h_file << type_declr << '\n';
    }

    for (const std::string& struct_declr : forward_declrs.struct_declrs)
    {
        h_file << struct_declr << '\n';
    }

    for (const std::string& fn_declr : forward_declrs.function_declrs)
    {
        h_file << fn_declr << '\n';
    }

    for (const std::string& global_var : forward_declrs.global_declrs)
    {
        h_file << global_var << '\n';
    }

    h_file << "#endif" << '\n';

    h_file.close();
    c_file.close();
    src_file.close();

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Error: Missing command line arguments after boris...");
        return 1;
    }
    else if (argc == 2)
    {
        if (strncmp(argv[1], "--version", 10))
        {
            fprintf(stderr, "Error: Expected argument --version after boris.");
            return 1;
        }
        printf("%s", BORIS_VERSION);
    }
    else if (argc > 2)
    {
        if (!strncmp(argv[1], "compile", 8))
        {
            translate_file(std::string(argv[2]));
        }
        else
        {
            fprintf(stderr, "Error: Expected second argument to be \"compile\", instead found \"%s\"", argv[1]);
            return 1;
        }
    }
    return 0;
}
