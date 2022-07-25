// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <iostream>
#include <chrono>
#include <fstream>
#include <array>
#include <deque>
#include "options_parser.h"
#include "methods.h"

size_t method1_easy_easy(std::ifstream &file, std::string *str_res) {
    if (!str_res) {
        return NULLPTR_ERROR;
    }
    // good because it will be highly optimized
    auto stream = std::ostringstream{};
    stream << file.rdbuf();

    if (file.fail()) {

        return ERROR_READING_INPUT_FILE;
    }

    *str_res = stream.str();
    return 0;
}

size_t method2_ignore(std::ifstream &file, std::string *str_res) {
    if (!str_res) {
        return NULLPTR_ERROR;
    }
    // tell the starting position
    auto const beginning = file.tellg();

    // to ignore characters it should read it
    file.ignore(std::numeric_limits<std::streamsize>::max());

    if (file.fail()) {
        return ERROR_READING_INPUT_FILE;
    }

    // tells the number of characters in the last read operation
    auto const num_of_chars = file.gcount();

    // return to the beginning of the file
    file.seekg(beginning);

    // create string
    *str_res = std::string(num_of_chars, char{});

    // one big monstrous read
    file.read(&str_res->at(0), str_res->size());

    return 0;
}


/*
The Chunk + Deque Method.
We read the input stream by chunks, and while chunk is an array, we
can insert what is in the chunk into the deque, without full memory reallocation
and N/2 shifts max.
*/
size_t method3_chunk_read(std::ifstream &file, std::deque<char> *storage) {
    if (!storage) {
        return NULLPTR_ERROR;
    }
    // We get the BUFSIZ of the IO Stream
    auto const chunk_size = size_t{BUFSIZ};
    // Create a chunck the size of our bufsize
    auto chunk = std::array<char, chunk_size>{};
    // The deque that is used for storing what is read by chunks
    *storage = std::deque<char>();

    // The chunk reading, if we have a chunk to read then read it, or if we have
    // less than a chunk to read, then read what's left
    while (file.read(chunk.data(), chunk.size()) || file.gcount()) {
        storage->insert(std::end(*storage), std::begin(chunk), std::begin(chunk) + file.gcount());
    }

    if (!file.eof()) {
        return ERROR_READING_INPUT_FILE;
    }

    return 0;
}

/*
VERY BAD METHOD - Stream Iterators
This method is very slow as it basically reads char by char.
*/
size_t method4_stream_iterators(std::ifstream &file, std::string *data) {
    if (!data) {
        return NULLPTR_ERROR;
    }
    std::copy(std::istreambuf_iterator<char>{file},
              std::istreambuf_iterator<char>{},
              std::back_inserter(*data));

    if (file.fail()) {
        return ERROR_READING_INPUT_FILE;
    }

    return 0;
}

/*
Count the number of rows and the average row length by simply iterating over a string
*/
std::pair<unsigned int, double> operate_on_data(std::string &data) {
    unsigned int row_count = 0;
    double char_count = 0;

    std::pair<unsigned int, double> res_calc;
    for (char i : data) {
        if (i == '\n') {
            row_count++;
        } else {
            char_count++;
        }
    }
    res_calc.first = row_count;
    res_calc.second = char_count / static_cast<double>(row_count);

    return res_calc;
}

size_t write_data_to_file(const std::string &out_file_name, std::pair<unsigned int, double> results) {
    std::ofstream out_file(out_file_name);

    if (!out_file) {
        return ERROR_OPENING_OUTPUT_FILE;
    }

    out_file << std::fixed << results.second << '\n' << results.first << std::endl;

    if (out_file.fail()) {
        return ERROR_WRITING_TO_OUTPUT_FILE;
    }

    return 0;
}

// Just a simple deque to string converter for method 3
std::string deque_to_string(std::deque<char> &deque) {
    std::string result_string(deque.begin(), deque.end());
    return result_string;
}