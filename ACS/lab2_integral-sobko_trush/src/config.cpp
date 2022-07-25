// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "config.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

void config_t::read_config(std::istream &config_file) {
    try {
        po::options_description opt_conf{"Config File Options\n"};
        opt_conf.add_options()
                ("abs_err", po::value<double>(&abs_err)->required(), "Absolute error")
                ("rel_err", po::value<double>(&rel_err)->required(), "Relative error")
                ("x_start", po::value<double>(&x_start)->required(), "Start of the interval by the x axis")
                ("x_end", po::value<double>(&x_end)->required(), "End of the interval by the x axis")
                ("y_start", po::value<double>(&y_start)->required(), "Start of the interval by the y axis")
                ("y_end", po::value<double>(&y_end)->required(), "End of the interval by the y axis")
                ("n_threads", po::value<size_t>(&n_threads)->required(), "Number of threads")
                ("init_steps_x", po::value<size_t>(&init_steps_x)->required(), "Initial number of points by the x axis")
                ("init_steps_y", po::value<size_t>(&init_steps_y)->required(), "Initial number of points by the y axis")
                ("max_iter", po::value<size_t>(&max_iter)->required(), "Maximum number of iterations");

        po::parsed_options parsed = po::parse_config_file(config_file, opt_conf);
        po::store(parsed, var_map);
        po::notify(var_map);
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        std::exit(CONFIG_FILE_ERROR);
    }
}

config_t::config_t(std::istream &file_stream) {
    read_config(file_stream);
}
