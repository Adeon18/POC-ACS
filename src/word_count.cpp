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

    std::unordered_map<std::string, size_t> unzip_index_and_get_map(const std::pair<std::string, std::string>& file_content) {

        std::unordered_map<std::string, size_t> local_map;

        char* buffer = new char[size_limits::FILE_SIZE_LIMIT_BYTES];

        if (file_content.second == ".txt") {
            local_map = count_words(file_content.first.cbegin(), file_content.first.cend());
        } else if (file_content.second == ".zip") {
            local_map = archive_manager::unzip_and_index(file_content.first,  buffer);
        }
        delete [] buffer;

//        for (auto const & val: local_map) {
//            std::cout << val.first << ": " << val.second << std::endl;
//        }

        return local_map;
    }

    /*
    * Write mt_unordered_map to file sorted alphabetically by key.
    */
    void write_map_sorted_by_key(std::unordered_map<std::string, size_t> &global_map, const std::string &file_path) {
        write_sorted_map_to_file(global_map,
                                 [](std::pair<std::string, size_t> &word1, std::pair<std::string, size_t> &word2) {
                                     return word1.first.compare(word2.first) < 0;
                                 },
                                 file_path);
    }

    /*
    * Write mt_unordered_map to file sorted by value.
    */
    void write_map_sorted_by_value(std::unordered_map<std::string, size_t> &global_map, const std::string &file_path) {
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