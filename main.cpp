#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>

const std::regex white_space = std::regex("\\s");
const std::regex empty_line = std::regex("");
const std::regex if_statement = std::regex("^if\\s+.+?");
const std::regex else_if_statement = std::regex("else\\s*if\\s+(.+)");
const std::regex else_statement = std::regex("else\\s*");
const std::regex assignment = std::regex("^[_a-zA-Z][_a-zA-Z0-9]*\\s*=\\s*.+$");
const std::regex mutable_initialization = std::regex("^\s*let\s+mut\s+[\w_]+\s*(?::\s*[\w_]+)?\s*(=\s*[^;]+)");
const std::regex default_initialization = std::regex("^\s*let\s+[\w_]+\s*(?::\s*[\w_]+)?\s*(=\s*[^;]+)$");
const std::regex typed_initialization = std::regex("^[_a-zA-Z][_a-zA-Z0-9]*\\s*:\\s*(?:.|\\n)*=\\s*(?:.|\\n)*$");
const std::regex typed_param = std::regex("^[_a-zA-Z][_a-zA-Z0-9]*\\s*:\\s*(?:.|\\n)*");
const std::regex template_param = std::regex("^[_a-zA-Z][_a-zA-Z0-9]*\\s*");
const std::regex fn_header = std::regex("^fn\\s+[_a-zA-Z][_a-zA-Z0-9]*\\s*\\((?:.|\n)*\\)\\s*(?:->\\s*(?:.|\n)*)?\\s*$");
const std::regex for_each_loop = std::regex("for\\s+(\\w+)\\s+in\\s+(.+)");
const std::regex while_loop = std::regex("while\\s+(.+)");
const std::regex struct_pattern = std::regex("^\s*struct\s+[\w_]+\s*(\([\w_,\s]*\))?\s*");
const std::regex trait_pattern = std::regex("^\s*trait\s+[\w_]+\s*");
const std::regex fn_call = std::regex("([a-zA-Z_][a-zA-Z0-9_]*\\.)+[a-zA-Z_][a-zA-Z0-9_]*(\\([^\\)]*\\)\\.)+[a-zA-Z_][a-zA-Z0-9_]*\\([^\\)]*\\)");

const char* std_headers[] = 
{
    "iostream", "concepts", "coroutine", "any", "bitset", "chrono", "compare",
    "csetjmp", "csignal", "cstdarg", "cstddef", "cstdlib", "ctime",
    "expected", "functional", "initializer_list", "optional", "vector"
};

class BorisStr
{
    private:
    std::string string;

    public:

    BorisStr(std::string string): string(string){}

    BorisStr(const char* cstr): string(cstr){}

    friend BorisStr operator +(const BorisStr& self, const BorisStr& other)
    {
        return BorisStr(self.string + other.string);
    }

    friend BorisStr operator +=(const BorisStr& self, const BorisStr& other)
    {
        return self + other;
    }

    friend bool operator == (const BorisStr& self, const BorisStr& other)
    {
        return self.string == other.string;
    }

    char operator [](int index)
    {
        if (index < string.length())
        {
            return string[index];
        }
        else
        {
            std::cout << "Accessed out of bounds of the string";
            std::exit(-1);
        }
    }

    std::string& get_CPP_str()
    {
        return string;
    }

    int len()
    {
        return string.size();
    }

    std::vector<BorisStr> split(const std::string& delimiter)
    {
        std::vector<BorisStr> return_vec = {};
        std::string copy = string;
        size_t pos = 0;
        while((pos = copy.find(delimiter)) != std::string::npos)
        {
            std::string token = copy.substr(0, pos);
            return_vec.push_back(token);
            copy.erase(0, pos + delimiter.length());
        }
        return_vec.push_back(copy);
        return return_vec;
    }

    void remove(const std::string& erasure)
    {
        size_t index_of = 0;
        std::string new_str = "";
        while((index_of = string.find(erasure)) != std::string::npos)
        {
            new_str += string.substr(0, index_of);
            string.erase(0, index_of + erasure.length());
        }
        new_str += string;
        string = new_str;
    }

    void remove_first_occurence(const std::string& erasure)
    {
        size_t index_of = string.find(erasure);
        if (index_of != std::string::npos)
        {
            string = string.substr(0, index_of) + string.substr(index_of + erasure.length(), string.length());
        }
    }

    void replace(const std::string& erasure, const std::string& replacement)
    {
        size_t index_of = 0;
        std::string new_str = "";
        while((index_of = string.find(erasure)) != std::string::npos)
        {
            new_str += string.substr(0, index_of) + replacement;
            string.erase(0, index_of + erasure.length());
        }
        new_str += string;
        string = new_str;
    }

    void strip_all_white_space()
    {
        replace(" ", "");
        replace("\t", "");
        replace("\r", "");
        replace("\n", "");
    }

    void strip()
    {
        std::string stripped_copy = "";
        int i = 0;

        for (; i < string.length(); i++)
        {
            if (string[i] != '\t' && string[i] != '\r' && string[i] != ' ' && string[i] != '\n')
            {
                break;
            }
        }

        int k = string.length() - 1;

        for (; k >= 0; k--)
        {
            if (string[k] != '\t' && string[k] != '\r' && string[k] != ' ' && string[k] != '\n')
            {
                break;
            }
        }

        stripped_copy += string.substr(i, k + 1);

        string = stripped_copy;
    }
};

class Generator
{
    private:
    std::ifstream& src_file;
    std::ofstream& cpp_file;
    std::vector<std::string> vars;
    int indent_count = 0;

    std::string add_tabs(const std::string& line)
    {
        if (indent_count)
        {
            std::string adjusted_line = "";
            for (int i = 0; i < indent_count; i++)
            {
                adjusted_line += "\t";
            }
            adjusted_line += line;
            return adjusted_line;
        }
        return line;
    }

    void write_line(const std::string& line)
    {
        std::string line_to_write = add_tabs(line);
        cpp_file << line_to_write << "\n";
    }

    bool is_std_header(const std::string& line)
    {
        for (auto header : std_headers)
        {
            if (line == header)
            {
                return true;
            }
        }
        return false;
    }

    const int count_tabs(BorisStr& line)
    {
        for (int i = 0; i < line.len(); i++)
        {
            if (line[i] != '\t')
            {
                return i;
            }
        }
        return 0;
    }

    void remove_local_vars(std::vector<std::string>& local_vars)
    {
        std::vector<std::string> adjusted_vars = {};
        for (auto str : vars)
        {
            if (std::find(local_vars.begin(), local_vars.end(), str) != local_vars.end())
            {
                continue;
            }
            adjusted_vars.push_back(str);
        }
        vars = adjusted_vars;
    }

    void parse_fn_params(BorisStr& fun_header)
    {
        bool templated = false;
        std::string template_adjustment = "template <";
        std::string adjusted_fun = "";

        std::vector<BorisStr> fun_info = fun_header.split("(");

        BorisStr fun_params_info = fun_info[1];

        fun_params_info.replace(")", "");

        std::vector<BorisStr> params = fun_params_info.split(",");

        std::string adjusted_params = "";

        int template_counter = 0;

        for (int i = 0; i < params.size(); i++)
        {
            BorisStr param = params[i];

            const char* const_qualifier = "const";

            if (std::find(param.get_CPP_str().begin(), param.get_CPP_str().end(), "mut") != param.get_CPP_str().end())
            {
                const_qualifier = "";
            }
            
            param.remove("mut");

            param.strip_all_white_space();

            if (std::regex_match(param.get_CPP_str(), typed_param))
            {
                std::string type_name = "";

                int index_of = param.get_CPP_str().find(':');

                for (int k = index_of + 1; k < param.len(); k++)
                {
                    type_name += param[k];
                }

                param.remove(":" + type_name);

                param = BorisStr(const_qualifier + type_name + " " + param);
            }
            else if (std::regex_match(param.get_CPP_str(), template_param))
            {
                templated = true;
                template_counter++;

                std::string type_name = "T" + std::to_string(template_counter);
                param = type_name + " " + param;

                template_adjustment += i != params.size() - 1 ? "typename " + type_name + ", " : "typename " + type_name;
            }

            adjusted_params += i != params.size() - 1 ? param.get_CPP_str() + ", " : param.get_CPP_str();
        }

        if (!templated)
        {
            template_adjustment = "";
        }
        else
        {
            template_adjustment += ">\n";
        }
        adjusted_params = "(" + adjusted_params + ")";

        adjusted_fun = template_adjustment + fun_info[0].get_CPP_str() + adjusted_params;

        fun_header = adjusted_fun;
    }

    void parse_fn_return_type(BorisStr& fun_header)
    {
        if (fun_header.get_CPP_str().find("->") != std::string::npos)
        {
            std::vector<BorisStr> split_str = fun_header.split("->");
            BorisStr fun_head = split_str[0];
            split_str[1].strip_all_white_space();
            fun_head.replace("fn", split_str[1].get_CPP_str());
            return;
        }

        fun_header.replace("fn", "inline auto");
    }

    std::string generate_single_line(std::string line)
    {
        if (std::regex_match(line, empty_line) || std::regex_match(line, white_space))
        {
            write_line("");
            return "";
        }

        BorisStr adjust_line = BorisStr(line);

        const int tab_num = count_tabs(adjust_line);

        if (tab_num < indent_count)
        {
            indent_count = tab_num;
            return line;
        }

        adjust_line.strip();

        line = adjust_line.get_CPP_str();

        if (std::regex_match(line, assignment))
        {
            // std::string line_to_write = "";
            // std::string value = "";
            // std::string var_name = "";

            // std::vector<BorisStr> split = adjust_line.split("=");

            // var_name = split[0].get_CPP_str();
            // value = split[1].get_CPP_str();

            // const bool var_already_exists = std::find(vars.begin(), vars.end(), var_name) != vars.end();

            // if (!var_already_exists)
            // {
            //     line_to_write += "auto " + var_name + "=" + value + ";";
            //     write_line(line_to_write);
            // }
            // else
            // {
            //     line += ";";
            //     write_line(line);
            // }

            //TODO: Implement type constructor
            write_line(adjust_line.get_CPP_str());
        }
        else if (std::regex_match(line, default_initialization))
        {
            adjust_line.replace("let", "const auto");
            //TODO: Implement type constructor
            write_line(adjust_line.get_CPP_str());
        }
        else if (std::regex_match(line, mutable_initialization))
        {
            adjust_line.remove(" mut");
            adjust_line.replace("let", "auto");
            write_line(adjust_line.get_CPP_str());
        }
        else if (std::regex_match(line, fn_header))
        {
            parse_fn_return_type(adjust_line);
            parse_fn_params(adjust_line);
            write_line("{");
            indent_count++;
            std::string next_line = generate_until_tab_break();
            write_line("}");
            generate_single_line(next_line);
        }
        else if (std::regex_match(line, while_loop))
        {
            adjust_line.replace("while", "while (");
            adjust_line.get_CPP_str().push_back(')');
            write_line(adjust_line.get_CPP_str());
            write_line("{");
            indent_count++;
            std::string next_line = generate_until_tab_break();
            write_line("}");
            generate_single_line(next_line);
        }
        else if (std::regex_match(line, for_each_loop))
        {
            adjust_line.replace("for", "for (");
            adjust_line.get_CPP_str().push_back(')');
            adjust_line.replace("in", ":");
            write_line(adjust_line.get_CPP_str());
            write_line("{");
            indent_count++;
            std::string next_line = generate_until_tab_break();
            write_line("}");
            generate_single_line(next_line);
        }
        else if (std::regex_match(line, struct_pattern))
        {
            adjust_line.replace("(", " : public ");
            adjust_line.remove(")");
            write_line(adjust_line.get_CPP_str());
            write_line("{");
            indent_count++;
            std::string next_line = generate_until_tab_break();
            write_line("};");
            generate_single_line(next_line);
        }
        else if (std::regex_match(line, trait_pattern))
        {
            adjust_line.replace("trait", "struct");
            write_line(adjust_line.get_CPP_str());
            write_line("{");
            indent_count++;
            std::string next_line = generate_until_tab_break();
            write_line("};");
            generate_single_line(next_line);
        }
        else
        {
            write_line(line + ";");
        }
    }

    std::string generate_until_tab_break()
    {
        std::vector<std::string> local_variables = {};
        std::string line = "";

        while (std::getline(src_file, line))
        {
            if (std::regex_match(line, empty_line) || std::regex_match(line, white_space))
            {
                write_line("");
                continue;
            }

            BorisStr adjust_line = BorisStr(line);

            const int tab_num = count_tabs(adjust_line);

            if (tab_num != indent_count)
            {
                indent_count = tab_num;
                remove_local_vars(local_variables);
                return line;
            }

            adjust_line.strip();

            line = adjust_line.get_CPP_str();

            if (line.find("include") != std::string::npos)
            {
                adjust_line.replace("include ", "");
                if (is_std_header(adjust_line.get_CPP_str()))
                {
                    write_line("#include <" + adjust_line.get_CPP_str() + ">");
                }
                else
                {
                    write_line("#include \"" + adjust_line.get_CPP_str() + "\"");
                }
            }
            else if (line.find("main") != std::string::npos)
            {
                write_line("int main(int argc, char* argv[])\n{");
                indent_count++;
                generate_until_tab_break();
                write_line("}");
            }
            else if (std::regex_match(line, assignment))
            {
                std::string line_to_write = "";
                std::string value = "";
                std::string var_name = "";

                std::vector<BorisStr> split = adjust_line.split("=");

                var_name = split[0].get_CPP_str();
                value = split[1].get_CPP_str();

                const bool var_already_exists = std::find(vars.begin(), vars.end(), var_name) != vars.end();

                if (!var_already_exists)
                {
                    line_to_write += "auto " + var_name + "=" + value + ";";
                    write_line(line_to_write);
                }
                else
                {
                    line += ";";
                    write_line(line);
                }
            }
            else if (std::regex_match(line, fn_header))
            {
                parse_fn_return_type(adjust_line);
                parse_fn_params(adjust_line);
                write_line("{");
                indent_count++;
                std::string next_line = generate_until_tab_break();
                write_line("}");
                generate_single_line(next_line);
            }
            else if (std::regex_match(line, while_loop))
            {
                adjust_line.replace("while", "while (");
                adjust_line.get_CPP_str().push_back(')');
                write_line(adjust_line.get_CPP_str());
                write_line("{");
                indent_count++;
                std::string next_line = generate_until_tab_break();
                write_line("}");
                generate_single_line(next_line);
            }
            else if (std::regex_match(line, for_each_loop))
            {
                adjust_line.replace("for", "for (");
                adjust_line.get_CPP_str().push_back(')');
                adjust_line.replace("in", ":");
                write_line(adjust_line.get_CPP_str());
                write_line("{");
                indent_count++;
                std::string next_line = generate_until_tab_break();
                write_line("}");
                generate_single_line(next_line);
            }
            else if (std::regex_match(line, struct_pattern))
            {
                adjust_line.replace("(", " : public ");
                adjust_line.remove(")");
                write_line(adjust_line.get_CPP_str());
                write_line("{");
                indent_count++;
                std::string next_line = generate_until_tab_break();
                write_line("};");
                generate_single_line(next_line);
            }
            else if (std::regex_match(line, trait_pattern))
            {
                adjust_line.replace("trait", "struct");
                write_line(adjust_line.get_CPP_str());
                write_line("{");
                indent_count++;
                std::string next_line = generate_until_tab_break();
                write_line("};");
                generate_single_line(next_line);
            }
            else
            {
                write_line(line + ";");
            }
        }
        return "";
    }

    public:

    Generator(std::ifstream& src_file, std::ofstream& cpp_file): src_file(src_file), cpp_file(cpp_file) {}
    Generator(const Generator&) = default;
    Generator(const Generator&&) = delete;
    Generator operator = (const Generator&) = delete;
    Generator operator = (const Generator&&) = delete;
    ~Generator() = default;

    void generate()
    {
        std::vector<std::string> local_variables = {};
        std::string line = "";

        while (std::getline(src_file, line))
        {
            BorisStr adjust_line = BorisStr(line);

            if (line.find("include") != std::string::npos)
            {
                adjust_line.replace("include ", "");
                if (is_std_header(adjust_line.get_CPP_str()))
                {
                    write_line("#include <" + adjust_line.get_CPP_str() + ">");
                }
                else
                {
                    write_line("#include \"" + adjust_line.get_CPP_str() + "\"");
                }
            }
            else if (line.find("main") != std::string::npos)
            {
                write_line("int main(int argc, char* argv[])\n{");
                indent_count++;
                generate_until_tab_break();
                write_line("}");
            }
            else if (std::regex_match(line, assignment))
            {
                std::string line_to_write = "";
                std::string value = "";
                std::string var_name = "";

                std::vector<BorisStr> split = adjust_line.split("=");

                split[0].strip();
                split[1].strip();

                var_name = split[0].get_CPP_str();
                value = split[1].get_CPP_str();

                const bool var_already_exists = std::find(local_variables.begin(), local_variables.end(), var_name) != local_variables.end();

                if (!var_already_exists)
                {
                    line_to_write += "inline auto " + var_name + " = " + value + ";";
                    write_line(line_to_write);
                }
                else
                {

                }
            }
            else if (std::regex_match(line, fn_header))
            {
                parse_fn_return_type(adjust_line);
                parse_fn_params(adjust_line);
                write_line(adjust_line.get_CPP_str());
                indent_count++;
                write_line("{");
                std::string next_line = generate_until_tab_break();
                write_line("}");
                generate_single_line(next_line);
            }
            else if (std::regex_match(line, empty_line))
            {
                write_line("");
            }
            else
            {
                write_line(line + ";");
            }
        }
    }
};

int main()
{
    std::string file_name;
    std::getline(std::cin, file_name);
    std::ifstream src_file = std::ifstream();
    src_file.open(file_name + ".boris");

    std::ofstream cpp_file = std::ofstream(file_name + ".cpp");

    Generator cpp_generator = Generator(src_file, cpp_file);

    cpp_generator.generate();

    cpp_file.close();
    src_file.close();

    return 0;
}