// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <mutex>
#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>
#include <functional>
#include "word_count.hpp"
#include "exit_codes.hpp"
#include "archive_manager.hpp"
#include <boost/algorithm/string.hpp>

namespace word_count {
    /*
    * Write mt_unordered_map to file sorted alphabetically by key.
    */
    void write_map_sorted_by_key(chm &global_map, const std::string &file_path) {
        write_sorted_map_to_file(global_map,
                                 [](std::pair<std::string, size_t> &word1, std::pair<std::string, size_t> &word2) {
                                     return word1.first.compare(word2.first) < 0;
                                 },
                                 file_path);
    }

    /*
    * Write mt_unordered_map to file sorted by value.
    */
    void write_map_sorted_by_value(chm &global_map, const std::string &file_path) {
        write_sorted_map_to_file(global_map,
                                 [](std::pair<std::string, size_t> &word1, std::pair<std::string, size_t> &word2) {
                                     if (word1.second != word2.second) {
                                         return word1.second > word2.second;
                                     } else {
                                         return word1.first.compare(word2.first) < 0;
                                     }
                                 },
                                 file_path);
    }
}