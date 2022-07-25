#ifndef _PARALLEL_MERGE_HPP
#define _PARALLEL_MERGE_HPP

#include <string>
#include <unordered_map>

#include "mt_queue.hpp"
#include "mt_queue_multi.hpp"

namespace parallel_merge {
    typedef std::unordered_map<std::string, size_t> um;
    using mt_queue::mt_queue_t;
    using mt_queue_multi::mt_queue_multi_t;
    void merge(mt_queue_t<um>& mt_d_indexes, mt_queue_multi_t<um>& mt_qm_indexes);
}

#endif  // _PARALLEL_MERGE_HPP