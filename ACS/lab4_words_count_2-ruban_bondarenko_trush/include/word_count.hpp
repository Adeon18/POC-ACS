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
#include <boost/locale.hpp>

#include "mt_queue.hpp"
#include "exit_codes.hpp"
#include "custom_types.hpp"
#include "mt_queue_multi.hpp"

namespace word_count {
    using mt_queue::mt_queue_t;
    using custom_types::emptyable_pair;
    namespace ba = boost::locale::boundary;
    
    /*
    * Count individual words in a string, merge their count with the global map
    */
    template<typename iterator_type>
    void count_words(iterator_type contents_b, iterator_type contents_e,
            mt_queue_t<std::unordered_map<std::string, size_t>>& mt_d_indexes) {
        ba::segment_index<iterator_type> map(ba::word, contents_b, contents_e);
        map.rule(ba::word_letters);
        std::unordered_map<std::string, size_t> local_map;

        for (auto i = map.begin(), e = map.end(); i != e; ++i) {
            ++local_map[boost::locale::fold_case(boost::locale::normalize(i->str()))];
        }

        mt_d_indexes.push(std::move(local_map));
    }


    void index_files_from_deque(mt_queue_t<emptyable_pair<std::string, std::string>> &mt_d_file_contents,
                                mt_queue_t<std::unordered_map<std::string, size_t>>& mt_d_indexes);


    void write_map_sorted_by_key(std::unordered_map<std::string, size_t>& global_map,
                                 const std::string & file_path);

    void write_map_sorted_by_value(std::unordered_map<std::string, size_t>& global_map,
                                   const std::string & file_path);

    /*
    * Write mt_unordered_map to file sorted by condition set with function f.
    */
    template <typename F>
    void write_sorted_map_to_file(std::unordered_map<std::string, size_t>& global_map,
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
