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
#include "exit_codes.hpp"
#include <archive_entry.h>

namespace archive_manager {
    using chm = tbb::concurrent_hash_map<std::string, size_t>;
    std::unordered_map<std::string, size_t> unzip_and_index(const std::string &archive, char* buffer);
}

#endif //WORD_INDEXER_ARCHIVE_MANAGER_HPP