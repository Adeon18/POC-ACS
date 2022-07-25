// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "merge.hpp"

namespace merge {
    void merge_dictionaries(um &main_dict, const um &dict_to_merge) {
        for (const auto & [key, val]: dict_to_merge) {
            main_dict[key] += val;
        }
    }
}