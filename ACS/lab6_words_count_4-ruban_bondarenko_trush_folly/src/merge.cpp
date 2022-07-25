#include "merge.hpp"

namespace merge {
    std::unordered_map<std::string, size_t> merge_dictionaries(std::unordered_map<std::string, size_t> main_dict, const std::unordered_map<std::string, size_t> &dict_to_merge) {
        for (const auto & [key, val]: dict_to_merge) {
            main_dict[key] += val;
        }

        return main_dict;
    }
}