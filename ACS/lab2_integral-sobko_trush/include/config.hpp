// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef INTEGRATE_CONFIG_HPP
#define INTEGRATE_CONFIG_HPP

#include <cstddef>
#include <iosfwd>
#include <boost/program_options/variables_map.hpp>

enum ConfigErrors {
    CONFIG_FILE_ERROR = 3,
};

class config_t {
public:
    double rel_err = 0.0, abs_err = 0.0;
    double x_start = 0.0, x_end = 0.0;
    double y_start = 0.0, y_end = 0.0;
    size_t n_threads = 0;
    size_t init_steps_x = 0;
    size_t init_steps_y = 0;
    size_t max_iter = 0;

    explicit config_t(std::istream &file_stream);

private:
    boost::program_options::variables_map var_map{};

    void read_config(std::istream &config_file);
};

#endif //INTEGRATE_CONFIG_HPP
