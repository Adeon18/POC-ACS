#ifndef _TOML_PARSER_HPP
#define _TOML_PARSER_HPP

#include <string>

namespace toml_parser {
    typedef struct config_t {
        std::string indir;
        std::string out_by_a;
        std::string out_by_n;
        unsigned long indexing_threads;
        unsigned long merging_threads;
        unsigned long max_file_contents_capacity;
        unsigned long max_filename_capacity;
    } config_t;

    enum CONFIG_READ_STATUS {
        STATUS_OK, STATUS_OPEN_ERROR, STATUS_SYNTAX_ERROR
    };

    [[nodiscard]] CONFIG_READ_STATUS read_config(std::string filename, config_t& config);
}

#endif // _TOML_PARSER_HPP
