#ifndef _OPTIONS_PARSER_H
#define _OPTIONS_PARSER_H

#include <string>
#include <exception>
#include <stdexcept>
#include <boost/program_options.hpp>

class OptionsParseException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class command_line_options_t {
public:
    command_line_options_t();
    command_line_options_t(int ac, char **av);

    //! Explicit is better than implicit:
    command_line_options_t(const command_line_options_t&) = default;
    command_line_options_t& operator=(const command_line_options_t&) = delete;
    command_line_options_t(command_line_options_t&&) = default;
    command_line_options_t& operator=(command_line_options_t&&) = delete;
    ~command_line_options_t() = default;

    [[nodiscard]] std::string get_config_file() const { return filenames[0]; };

    void parse(int ac, char **av);
private:
    std::vector<std::string> filenames;

    boost::program_options::variables_map var_map{};
    boost::program_options::options_description opt_conf{
            "Options:\n\tword_indexer [-h|--help] <config>\n"};
};

#endif //_OPTIONS_PARSER_H