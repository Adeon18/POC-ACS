#ifndef _PARALLEL_MERGE_HPP
#define _PARALLEL_MERGE_HPP

#include <string>
#include <unordered_map>

#include "tbb/concurrent_queue.h"

namespace merge {
    typedef std::unordered_map<std::string, size_t> um;
    um merge_dictionaries(um main_dict, const um &dict_to_merge);
}

#endif  // _PARALLEL_MERGE_HPP