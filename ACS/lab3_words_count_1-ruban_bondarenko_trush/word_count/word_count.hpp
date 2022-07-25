// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by iamthewalrus on 3/27/22.
//

#ifndef TEMPLATE_WORD_COUNT_H
#define TEMPLATE_WORD_COUNT_H

#include <mutex>
#include <deque>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_map>
#include "../mt_deque/mt_deque.hpp"
#include "../include/exit_codes.hpp"
#include "../mt_unordered_map/mt_unordered_map_t.hpp"

namespace word_count {
    using mt_unordered_map::mt_unordered_map_t, mt_deque::mt_deque_t;
    void count_words(const std::string & contents,
                     mt_unordered_map_t<std::string, size_t> & global_map);

    void index_files_from_deque(mt_deque_t<std::string>& mt_d_file_contents,
                                mt_unordered_map_t<std::string, size_t>& global_map);

    void write_map_sorted_by_key(mt_unordered_map_t<std::string, size_t>& global_map,
                                 const std::string & file_path);

    void write_map_sorted_by_value(mt_unordered_map_t<std::string, size_t>& global_map,
                                   const std::string & file_path);

    /*
    * Write mt_unordered_map to file sorted by condition set with function f.
    */
    template <typename F>
    void write_sorted_map_to_file(mt_unordered_map_t<std::string, size_t>& global_map,
                                  F f, const std::string & file_path) {
        std::vector<std::pair<std::string, size_t> > words(global_map.cbegin(), global_map.cend());
        std::sort(words.begin(), words.end(), f);

        std::ofstream output_file{file_path, std::ios::out};
        if (!output_file) {
            std::cerr << "Error opening output file" << std::endl;
            exit(ERROR_OPENING_FILE);
        }

        for (const auto & word: words) {
            output_file << word.first << " " << word.second << std::endl;
            if (output_file.fail()) {
                std::cerr << "Error writing to output file";
                exit(ERROR_WRITING_TO_FILE);
            }
        }
    }
}

#endif //TEMPLATE_WORD_COUNT_H
