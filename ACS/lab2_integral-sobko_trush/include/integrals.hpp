// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#ifndef INTEGRATE_INTEGRALS_HPP
#define INTEGRATE_INTEGRALS_HPP

#include <memory>
#include "config.hpp"

double langermann_function(double x, double y);

// This template was inspired by the code, contributed by indrekis: https://github.com/indrekis/integrate1d_sample
template<typename func_T>
int integrate(func_T func, std::shared_ptr<config_t> config_ptr, const std::pair<size_t, size_t> &actual_steps, double &result, size_t id, const double &prev_result)
{
    result = 0;
    // Get the x start and y start and end(for the current thread.)
    double x = config_ptr->x_start;
    double y = config_ptr->y_start + static_cast<double>(id) * (config_ptr->y_end - config_ptr->y_start) / config_ptr->n_threads;
    double actual_y_end = y + (config_ptr->y_end - config_ptr->y_start) / config_ptr->n_threads;
    // Calculate step length
    double delta_x = (config_ptr->x_end - config_ptr->x_start) / actual_steps.first;
    double delta_y = (actual_y_end - y) / actual_steps.second;

    // We skip the calculation step if both x and y steps are even if we have a previous result
    // The PVS-Studio warnings here are false positive
    for (size_t cur_step_y = 0; cur_step_y < actual_steps.second; cur_step_y++) {
        for (size_t cur_step_x = 0; cur_step_x < actual_steps.first; cur_step_x++) {
            if ( prev_result == std::numeric_limits<double>::infinity() || !(cur_step_y % 2 == 0 && cur_step_x % 2 == 0)) {
                result += func(x + cur_step_x * delta_x, y + cur_step_y * delta_y);
            }
        }
    }

    result *= delta_x * delta_y;
    // We add a prev_res / 4 because we were skipping even indexes before
    if (prev_result != std::numeric_limits<double>::infinity()) {
        result += prev_result / 4;
    }

    return 0;
}

#endif //INTEGRATE_INTEGRALS_HPP
