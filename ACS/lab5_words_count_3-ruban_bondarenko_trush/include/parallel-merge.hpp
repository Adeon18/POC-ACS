#ifndef _PARALLEL_MERGE_HPP
#define _PARALLEL_MERGE_HPP

#include <string>
#include <unordered_map>

#include "tbb/concurrent_queue.h"

namespace parallel_merge {
    typedef std::unordered_map<std::string, size_t> um;
    void merge(tbb::concurrent_bounded_queue<um>& mt_q_indexes);
}

#endif  // _PARALLEL_MERGE_HPP