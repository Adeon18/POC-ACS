#include "toml_parser.hpp"

#include <boost/algorithm/string.hpp>
#include <functional>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <map>

constexpr char EQUALITY_DELIMITER = '=';
constexpr char COMMENT_DELIMITERS[3] = "#;";

const std::map<std::string, std::string> param_types_map = {
    {"indir", "std::string"},
    {"out_by_a", "std::string"},
    {"out_by_n", "std::string"},
    {"indexing_threads", "unsigned long int"}
};

class SyntaxErrorException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

void print_syntax_error(std::string line, size_t line_n, size_t pos) {
    std::cout << "Syntax Error: Wrong variable value format (line " << line_n << ")" << std::endl;
    std::cout << "    " << line << std::endl;
    std::string arrow(line.size() + 4, ' ');
    arrow[pos + 4] = '^';
    std::cout << arrow << std::endl;
}

void trim_commets_and_spaces(std::string& line) {
    line = line.substr(0, line.find_first_of(COMMENT_DELIMITERS));
    boost::trim(line);  // trim all space characters
}

struct param_str_d_t {
    std::string name;
    std::string type;
    std::string value;
    size_t delim_pos;

    param_str_d_t() {
        name = "";
        type = "";
        value = "";
        delim_pos = 0;
    }

    param_str_d_t(std::string name_, std::string type_, std::string value_, size_t delim_pos_) {
        name = name_;
        type = type_;
        value = value_;
        delim_pos = delim_pos_;
    }
};

param_str_d_t get_param_type_and_value(const std::string& line) {
    auto delim_pos = line.find(EQUALITY_DELIMITER);

    if (delim_pos == std::string::npos) return param_str_d_t();

    auto param_name = line.substr(0, delim_pos);
    boost::trim(param_name);
    boost::trim_if(param_name, boost::is_any_of("\t\n\v\f\r "));

    auto type_ptr = param_types_map.find(param_name);
    if (type_ptr == param_types_map.end()) return param_str_d_t();

    auto param_val = line.substr(delim_pos + 1);
    boost::trim(param_val);
    boost::trim_if(param_val, boost::is_any_of("\t\n\v\f\r "));

    return param_str_d_t{
        static_cast<std::string>(param_name),
        static_cast<std::string>(type_ptr->second),
        static_cast<std::string>(param_val),
        delim_pos
    };
}

bool test_incorrect_chars(param_str_d_t param_str_d, std::string line, size_t line_n, std::string accept) {
    auto invalid_char_p = std::find_if(
        param_str_d.value.begin(), param_str_d.value.end(),
        std::not_fn(boost::is_any_of(accept))
    );
    if (invalid_char_p != param_str_d.value.end()) {
        size_t position = param_str_d.delim_pos + 1;
        while (std::isspace(line[position])) ++position;
        print_syntax_error(line, line_n,
            position + static_cast<size_t>(invalid_char_p - param_str_d.value.begin()));
        return false;
    }
    return true;
}

void parse_string(toml_parser::config_t& config, param_str_d_t param_str_d) {
    if ((
        param_str_d.value.find("\"") == 0 && param_str_d.value.substr(1).find("\"") == (param_str_d.value.length() - 2)
    ) || (
        param_str_d.value.find("'") == 0 && param_str_d.value.substr(1).find("'") == (param_str_d.value.length() - 2)
    )) {
        param_str_d.value = param_str_d.value.substr(1, param_str_d.value.length() - 2);
    } else throw SyntaxErrorException("");

    if (param_str_d.name == "indir") {
        config.indir = param_str_d.value;
    } else if (param_str_d.name == "out_by_a") {
        config.out_by_a = param_str_d.value;
    } else if (param_str_d.name == "out_by_n") {
        config.out_by_n = param_str_d.value;
    }
}

void parse_ul(toml_parser::config_t& config, param_str_d_t param_str_d) {
    unsigned long value = std::stoul(param_str_d.value);
    if (param_str_d.name == "indexing_threads") {
        config.indexing_threads = value;
    }
}

namespace toml_parser {
    [[nodiscard]] CONFIG_READ_STATUS read_config(std::string filename, config_t& config) {
        std::ifstream fs_in;
        fs_in.open(filename, std::ifstream::in);
        if (!(fs_in.is_open())) {
            return STATUS_OPEN_ERROR;
        }
        std::string line;
        size_t line_n = 0;
        while (std::getline(fs_in, line)) {
            ++line_n;

            trim_commets_and_spaces(line);

            if (line.size() == 0) continue;

            param_str_d_t param_str_d = get_param_type_and_value(line);

            if (param_str_d.name == "") continue;

            if (param_str_d.type != "std::string" && !test_incorrect_chars(
                param_str_d, line, line_n, "1234567890.-"
            )) return STATUS_SYNTAX_ERROR;

            try {
                if (param_str_d.type == "std::string") {
                    parse_string(config, param_str_d);
                } else if (param_str_d.type == "unsigned long int") {
                    parse_ul(config, param_str_d);
                }
            } catch (const std::exception& e) {
                print_syntax_error(line, line_n, param_str_d.delim_pos + 1);
                return STATUS_SYNTAX_ERROR;
            }
        }
        return STATUS_OK;
    }
}