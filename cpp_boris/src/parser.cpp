// #include "parser.hpp"

// Parser::Parser(const char* file_name, std::vector<Token>& tokens)
// {
//     this->lines = file_line_by_line(tokens);
//     Parser::start_statements[static_cast<uint64_t>(TokenType::FN)] = Parser::func;
// }

// Parser::ParseFunc Parser::start_statements[static_cast<uint64_t>(TokenType::NUMBER_OF_TOKEN_TYPES)] = { nullptr };

// std::vector<Parser::Line> Parser::file_line_by_line(std::vector<Token>& tokens)
// {
//     std::vector<Parser::Line> lines = {};

//     std::vector<Token> line = {};

//     for (const Token& token : tokens)
//     {
//         if (token.token_type == TokenType::NEWLINE)
//         {
//             lines.push_back(line);
//             line.clear();
//             continue;
//         }
//         line.push_back(token);
//     }

//     return lines;
// }

// Module Parser::parse()
// {
//     this->root = Module{};
//     for (this->file_it = this->lines.begin(); this->file_it != this->lines.end(); this->file_it++)
//     {
//         const Line& line = *file_it;

//         if (line.size() == 1 && line[0].token_type == TokenType::TAB || line[0].token_type == TokenType::SPACES)
//         {
//             continue;
//         }
//         else
//         {
//             //Why would it be non whitespace and be valid without any other tokens
//             //TODO: error
//             debug_print("Invalid statement");
//         }

//         ParseFunc start_parse = Parser::start_statements[static_cast<uint64_t>(line[0].token_type)];

//         if (start_parse == nullptr)
//         {
//             debug_print("Not a valid starting statement");
//         }

//         Statement statement = (this->*start_parse)();

//         this->statements.push_back(statement);
//     }

//     //TODO: placeholder return
//     return root;
// }

// Statement Parser::func()
// {
//     Function func = Function{};
    
//     Line func_declr = *this->file_it;
//     LineIterator func_it = func_declr.begin();
//     func_it++;

//     if (func_it->token_type != TokenType::IDENTIFIER)
//     {
//         //TODO: Implement Proper identifier error
//         std::string message = "Expected identifier for function at line ";
//         message += func_it->line_num + '0';
//         debug_print(message.c_str());
//     }
    
//     func.name = func_it->content;
//     func_it++;

//     if (func_it->token_type == TokenType::LBRACKET)
//     {
//         do
//         {
//             func_it++;
//             if (func_it->token_type == TokenType::IDENTIFIER)
//             {
//                 //TODO: add generics parse
//                 // Type generic = this->parse_type(func_it);
//                 // Statement generic_type = Statement{};
//                 // generic_type.type = StatementType::TYPE;
//                 // generic_type.value = generic;
//             }
//             else
//             {
//                 //TODO: error
//             }

//         } while (func_it->token_type == TokenType::COMMA);

//         if (func_it->token_type != TokenType::RBRACKET)
//         {
//             //TODO: error
//         }
//     }

//     if (func_it->token_type != TokenType::LPAREN)
//     {
//         //TODO: error for lack of paren
//         std::string message = "Expected open parenthesis for function " + func.name + " at line ";
//         message += func_it->line_num + '0';
//         debug_print(message.c_str());
//     }

//     func_it++;

//     func.parameters = this->func_parameters(func_it);

//     if (func_it->token_type != TokenType::RPAREN)
//     {
//         //TODO: error
//         //Missing rparen
//     }

//     func_it++;

//     func.return_type = this->func_return_type(func_it);

//     if (func_it->token_type != TokenType::COLON)
//     {
//         //TODO: error
//         //missing colon
//     }

//     //TODO: parse body

//     Statement func_statement = Statement{};
//     func_statement.type = StatementType::FUNCTION;
//     func_statement.value = func;

//     this->root.globals[func.name].value = func;

//     return func_statement;
// }

// std::vector<Value> Parser::func_parameters(LineIterator& func_it)
// {
//     const Token& paren_or_param_name = *func_it;

//     if (paren_or_param_name.token_type == TokenType::RPAREN)
//     {
//         return std::vector<Value>{};
//     }

//     if (paren_or_param_name.token_type != TokenType::IDENTIFIER)
//     {
//         //TODO: error
//         std::string message = "Expected closing parenthesis or parameter for function " + func_it->content + " at line ";
//         message += func_it->line_num + '0';
//         debug_print(message.c_str());
//     }

//     std::vector<Value> parameters = {};

//     do
//     {
//         Variable param = Variable{};
//         param.name = func_it->content;

//         //colon
//         func_it++;

//         if (func_it->token_type != TokenType::COLON)
//         {
//             //TODO: error missing colon
//         }

//         //on the type
//         func_it++;

//         Type param_type = this->parse_type(func_it);
//         param.data_type = param_type;

//         Value param_statement = Value{};
//         param_statement.type = ValueType::VARIABLE;
//         param_statement.value = param;

//         parameters.push_back(param_statement);

//         if (func_it->token_type != TokenType::COMMA)
//         {
//             break;
//         }

//         func_it++;

//     } while (true);

//     return parameters;
// }

// std::vector<Type> Parser::func_return_type(LineIterator& func_it)
// {
//     std::vector<Type> return_types = {};

//     do
//     {
//         Type return_type = this->parse_type(func_it);
//         return_types.push_back(return_type);

//         if (func_it->token_type != TokenType::COMMA)
//         {
//             break;
//         }

//         func_it++;
        
//     } while (func_it->token_type == TokenType::COMMA);
    
//     return return_types;
// }

// Type Parser::parse_type(LineIterator& line_it)
// {
//     Type type = Type{};

//     const Token& identifier = *line_it;

//     if (identifier.token_type != TokenType::IDENTIFIER)
//     {
//         //TODO: Error
//         //Missing identifier for type name
//     }

//     type.raw_type_name += identifier.content;
    
//     line_it++;

//     //module access
//     if (line_it->token_type == TokenType::DOT)
//     {
//         type.raw_type_name += line_it->content;

//         //skip the dot
//         line_it++;

//         // if (line_it->token_type != TokenType::IDENTIFIER)
//         // {
//         //     //TODO: error
//         //     //missing type, expected complete type name
//         // }

//         Type actual_type = this->parse_type(line_it);
//         actual_type.raw_type_name = type.raw_type_name + actual_type.raw_type_name;
//         actual_type.module_localizations.push_back(identifier.content);

//         return actual_type;
//     }
//     else if (line_it->token_type == TokenType::LBRACKET)
//     {
//         do
//         {
//             type.raw_type_name += line_it->content;

//             //skip the bracket, and the comma
//             line_it++;

//             Type type_param = this->parse_type(line_it);
//             type.nested_types.push_back(type_param);
//             type.raw_type_name += type_param.raw_type_name;
//         } while (line_it->token_type == TokenType::COMMA);

//         if (line_it->token_type != TokenType::RBRACKET)
//         {
//             //TODO: error
//             //generic type incomplete
//         }

//         type.raw_type_name += line_it->content;

//         line_it++;
//     }

//     type.core_type_name = identifier.content;

//     while (line_it->token_type == TokenType::STAR)
//     {
//         type.ptr_indirections++;
//         type.raw_type_name += line_it->content;       
//         line_it++;
//     }

//     return type;
// }

#include "parser.hpp"

Parser::Parser(const char* file_name, std::vector<std::vector<Token>>& tokens): file_name(file_name), tokens(tokens), index(0), line_num(0) {}

Token Parser::eat()
{
    Token previous = Token { TokenType::EOF_, this->line_num, "" };

    if (this->line_num >= this->tokens.size() - 1)
    {
        return previous;
    }

    if (this->index >= this->tokens[this->line_num].size() - 1)
    {
        this->index = 0;
        this->line_num++;
    }

    previous = this->tokens[this->line_num][this->index];

        

    return previous;
}

