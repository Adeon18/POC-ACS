// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef WORD_INDEXER_MERGE_HPP
#define WORD_INDEXER_MERGE_HPP

#include <iostream>
#include <unordered_map>

namespace merge {
    typedef std::unordered_map<std::string, size_t> um;
    void merge_dictionaries(um &main_dict, const um &dict_to_merge);
}

#endif //WORD_INDEXER_MERGE_HPP
