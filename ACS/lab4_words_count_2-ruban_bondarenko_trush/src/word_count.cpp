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
    Index files in the contents deque based on the contents being a .zip or .txt
    */
    void index_files_from_deque(mt_queue_t<emptyable_pair<std::string, std::string>> &mt_d_file_contents,
                                mt_queue_t<std::unordered_map<std::string, size_t>>& mt_d_indexes) {

        char* buffer = new char[size_limits::FILE_SIZE_LIMIT_BYTES];
        while (true) {
            auto file_contents = mt_d_file_contents.pop();

            if (file_contents.first.empty()) break;

            if (file_contents.second == ".txt") {
                count_words(
                    file_contents.first.cbegin(), file_contents.first.cend(),
                    mt_d_indexes
                );
            } else if (file_contents.second == ".zip") {
                archive_manager::unzip_and_index(file_contents.first, mt_d_indexes, buffer);
            }
        }
        delete [] buffer;

        mt_d_indexes.remove_data_creator();
    }

    /*
    * Write mt_unordered_map to file sorted alphabetically by key.
    */
    void write_map_sorted_by_key(std::unordered_map<std::string, size_t> &global_map,
                                 const std::string &file_path) {
        write_sorted_map_to_file(global_map,
                                 [](std::pair<std::string, size_t> &word1, std::pair<std::string, size_t> &word2) {
                                     return word1.first.compare(word2.first) < 0;
                                 },
                                 file_path);
    }

    /*
    * Write mt_unordered_map to file sorted by value.
    */
    void write_map_sorted_by_value(std::unordered_map<std::string, size_t> &global_map,
                                   const std::string &file_path) {
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