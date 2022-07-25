// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _LIST_AND_READ_HPP
#define _LIST_AND_READ_HPP

#include <boost/algorithm/string/predicate.hpp>
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_priority_queue.h"
#include "time_measurer.hpp"
#include "custom_types.hpp"
#include "limits.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <deque>
#include <array>

namespace list_and_read {
    namespace fs = std::filesystem;
    using boost::algorithm::ends_with;
    using custom_types::emptyable_pair;

    void add_files_to_queue(
        tbb::concurrent_bounded_queue<fs::path>& mt_d_filenames, const std::string &indir,
        time_measurer::mt_time_summmator_t &time_sum
    );

    std::string read_file_fast(std::ifstream& fs_in, bool check_size);

    std::pair<std::string, std::string> read_file_by_name(const fs::path &file_path);
}

#endif // _LIST_AND_READ_HPP