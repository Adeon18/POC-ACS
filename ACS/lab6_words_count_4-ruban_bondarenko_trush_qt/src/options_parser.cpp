// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "options_parser.hpp"
#include <filesystem>
#include <stdlib.h>
#include <iostream>

namespace po = boost::program_options;

command_line_options_t::command_line_options_t() {
    opt_conf.add_options()
        ("help,h", "Show help message");
}

command_line_options_t::command_line_options_t(int ac, char **av):
        command_line_options_t() // Delegate constructor
{
    parse(ac, av);
}

void command_line_options_t::parse(int ac, char **av) {
    try {
        po::parsed_options parsed = po::command_line_parser(ac, av).options(opt_conf).allow_unregistered().run();
        po::store(parsed, var_map);
        filenames = po::collect_unrecognized(parsed.options, po::include_positional);

        if (var_map.count("help")) {
            std::cout << opt_conf << "\n";
            exit(EXIT_SUCCESS);
        }
        if (filenames.size() > 1) {
            std::cout << "Error: Wrong number of arguments" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (filenames.size() >= 1 && !std::filesystem::exists(filenames[0])) {
            std::cout << "Error: Config file unreadible" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (filenames.size() == 0) {
            filenames.push_back("index.cfg");
        }
        po::notify(var_map);
    } catch (std::exception &ex) {
        throw OptionsParseException(ex.what()); // Convert to our error type
    }
}