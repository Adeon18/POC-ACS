// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <iostream>
#include <chrono>
#include <atomic>
#include <fstream>
#include <array>
#include <deque>
#include "options_parser.h"
#include "methods.h"

constexpr int DEQUE_METHOD_INDEX = 3;
constexpr int NUMBER_OF_ARGUMENTS = 4;

int main(int argv, char* argc[]) {
    // check number of arguments is valid
    if (argv != NUMBER_OF_ARGUMENTS) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return WRONG_NUM_ARGUMENTS;
    }

    size_t chosen_method = std::atoi(argc[1]);
    std::string input_file_name = argc[2];
    std::string output_file_name = argc[3];

    // open and check file
    std::ifstream inp_file(input_file_name);
    if (!inp_file) {
        std::cerr << "Failed opening input file" << std::endl;
        return ERROR_OPENING_INPUT_FILE;
    }

    // pair to store results of functions with different return types
    std::pair<std::string, std::deque<char>> data_from_file;
    size_t return_code;

    // start time and read file with chosen method
    auto time_start = get_current_time_fenced();
    switch (chosen_method) {
        case 1:
            return_code = method1_easy_easy(inp_file, &data_from_file.first);
            break;
        case 2:
            return_code = method2_ignore(inp_file, &data_from_file.first);
            break;
        case 3:
            return_code = method3_chunk_read(inp_file, &data_from_file.second);
            break;
        case 4:
            return_code = method4_stream_iterators(inp_file, &data_from_file.first);
            break;
        default:
        {
            std::cerr << "Wrong Method Specified" << std::endl;
            return WRONG_METHOD;
        }
    }
    auto time_end = get_current_time_fenced();
    auto time_taken = time_end - time_start;


    // handle errors
    if (return_code == ERROR_READING_INPUT_FILE) {
        std::cerr << "Failed reading input file" << std::endl;
        return ERROR_READING_INPUT_FILE;

    }
    
    std::cout << to_us(time_taken) << std::endl;

    // convert result of 3rd method to string
    if (chosen_method == DEQUE_METHOD_INDEX) {
        data_from_file.first = deque_to_string(data_from_file.second);
    }

    // count number of lines and calculate average length
    auto results = operate_on_data(data_from_file.first);
    return_code = write_data_to_file(output_file_name, results);


    // handle errors
    if (return_code == ERROR_OPENING_OUTPUT_FILE) {
        std::cerr << "Failed opening output file" << std::endl;
        return ERROR_OPENING_OUTPUT_FILE;

    } else if (return_code == ERROR_WRITING_TO_OUTPUT_FILE) {
        std::cerr << "Failed writing to output file" << std::endl;
        return ERROR_WRITING_TO_OUTPUT_FILE;
    }

    return 0;
}
