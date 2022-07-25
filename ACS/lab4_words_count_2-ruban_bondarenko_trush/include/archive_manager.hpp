// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef WORD_INDEXER_ARCHIVE_MANAGER_HPP
#define WORD_INDEXER_ARCHIVE_MANAGER_HPP

#include <mutex>
#include <deque>
#include <vector>
#include <fstream>
#include <functional>
#include <archive.h>
#include <unordered_map>
#include <archive_entry.h>
#include "mt_queue.hpp"
#include "exit_codes.hpp"
#include "parallel-merge.hpp"

namespace archive_manager {
    using parallel_merge::mt_queue_multi_t;
    using mt_queue::mt_queue_t;
    void unzip_and_index(const std::string &archive, mt_queue_t<std::unordered_map<std::string, size_t>>& mt_d_indexes, char* buffer);
}

#endif //WORD_INDEXER_ARCHIVE_MANAGER_HPP