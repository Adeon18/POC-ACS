// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <thread>
#include <numeric>
#include "integrals.hpp"
#include "time_counting.hpp"
#include "options_parser.h"
#include "config.hpp"

constexpr int VARIANT = 4;

enum MainErrors {
    INVALID_NUMBER_OF_ARGUMENTS = 1,
    CONFIG_FILE_OPENING_ERROR = 2,
    MAKE_SHARED_POINTER_ERROR = 4,
    REACHED_MAXIMUM_ITERATIONS_ERROR = 100,
};

int main(int argc, char* argv[]) {
    command_line_options_t command_line_options{argc, argv};
    if (argc != 2) {
        std::cerr << "Invalid number of command line arguments" << std::endl;
        return INVALID_NUMBER_OF_ARGUMENTS;
    }
    std::string config_file_name(command_line_options.get_filenames()[0]);
    std::ifstream config_stream(config_file_name);
    if (!config_stream.is_open()) {
        std::cerr << "Error opening config file" << std::endl;
        return CONFIG_FILE_OPENING_ERROR;
    }
    std::shared_ptr<config_t> config_ptr;
    try {
        config_ptr = std::make_shared<config_t>(config_stream);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return MAKE_SHARED_POINTER_ERROR;
    }

    // This pair stores the steps that always increase to new ones
    // This is done, because the value in config_t is stable
    std::pair<size_t, size_t> dynamic_steps{config_ptr->init_steps_x, config_ptr->init_steps_y};

    // 0 = 1 wow !
    if (config_ptr->n_threads == 0) {
        config_ptr->n_threads = 1;
    }

    double result = 0;
    double prev_result = std::numeric_limits<double>::infinity();

    bool continue_approximation = true;
    double absolute_error = -1;
    double relative_error = -1;
    size_t iteration_count = 1;

    auto start_time = get_current_time_fenced();
    // Regular
    if (config_ptr->n_threads <= 1) {
        integrate(langermann_function, config_ptr, dynamic_steps, result, 0, prev_result);
        prev_result = result;
        while (continue_approximation) {
#ifdef PRINT_INTERMEDIATE_STEPS
            std::cout << "---Step " << iteration_count << " ---" << std::endl;
            std::cout << std::setprecision(10) << "Result " << result << std::endl;
            std::cout << std::setprecision(10) << "Abs Error " << absolute_error << std::endl;
            std::cout << std::setprecision(10) << "Relative Error " << relative_error << std::endl;
#endif
            if (iteration_count == config_ptr->max_iter) {
                std::cerr << "Error: could not achieve the desired precision" << std::endl;
                return REACHED_MAXIMUM_ITERATIONS_ERROR;
            }
            dynamic_steps.first *= 2;
            dynamic_steps.second *= 2;
            // Reset the result
            result = 0;
            integrate(langermann_function, config_ptr, dynamic_steps, result, 0, prev_result);
            // Errors and Calculation Data
            absolute_error = fabs(result - prev_result);
            if (result != 0) {
                relative_error = fabs((result - prev_result) / result);
            } else {
                relative_error = std::numeric_limits<double>::infinity();
            }
            prev_result = result;
            ++iteration_count;
            // Exit if the errors are too small or we iterated enough times
            if (absolute_error < config_ptr->abs_err || relative_error < config_ptr->rel_err) {
                continue_approximation = false;
            }
        }
    // Parallel
    } else {
        dynamic_steps.second /= config_ptr->n_threads;
        std::vector<std::thread> threads;
        std::vector<double> results(config_ptr->n_threads, 0.0);
        std::vector<double> prev_results(config_ptr->n_threads, std::numeric_limits<double>::infinity());
        for (size_t i = 0; i < config_ptr->n_threads; ++i) {
            threads.emplace_back(integrate<double (*)(double, double)>, langermann_function, config_ptr,
                                 std::cref(dynamic_steps), std::ref(results[i]), i,
                                 std::cref(prev_results[i]));
        }
        // Threads joining
        for (auto &t: threads) {
            t.join();
        }
        threads.clear();
        result = std::accumulate(results.begin(), results.end(), 0.0);
        prev_result = result;
        prev_results = results;
        while (continue_approximation) {
#ifdef PRINT_INTERMEDIATE_STEPS
            std::cout << "---Step " << iteration_count << " ---" << std::endl;
            std::cout << std::setprecision(10) << "Result " << result << std::endl;
            std::cout << std::setprecision(10) << "Abs Error " << absolute_error << std::endl;
            std::cout << std::setprecision(10) << "Relative Error " << relative_error << std::endl;
#endif
            if (iteration_count == config_ptr->max_iter) {
                std::cerr << "Error: could not achieve the desired precision" << std::endl;
                return REACHED_MAXIMUM_ITERATIONS_ERROR;
            }
            dynamic_steps.first *= 2;
            dynamic_steps.second *= 2;
            // Threads management
            std::fill(results.begin(), results.end(), 0);
            for (size_t i = 0; i < config_ptr->n_threads; ++i) {
                threads.emplace_back(integrate<double (*)(double, double)>, langermann_function, config_ptr,
                                     std::cref(dynamic_steps), std::ref(results[i]), i,
                                     std::cref(prev_results[i]));
            }
            // Threads joining
            for (auto &t: threads) {
                t.join();
            }
            threads.clear();
            // Errors and Calculation Data
            result = std::accumulate(results.begin(), results.end(), 0.0);
            prev_results = results;
            absolute_error = fabs(result - prev_result);
            if (result != 0) {
                relative_error = fabs((result - prev_result) / result);
            } else {
                relative_error = std::numeric_limits<double>::infinity();
            }
            prev_result = result;
            // Exit if the errors are too small or we iterated enough times
            if (absolute_error < config_ptr->abs_err || relative_error < config_ptr->rel_err) {
                continue_approximation = false;
            }
        }
    }
    auto end_time = get_current_time_fenced();
    auto time_taken = end_time - start_time;

    // Result output
    std::cout << VARIANT << std::endl;
    std::cout << result << std::endl;
    std::cout << absolute_error << std::endl;
    std::cout << relative_error << std::endl;
    std::cout << to_us(time_taken) << std::endl;
    return 0;
}
