// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _LIST_AND_READ_HPP
#define _LIST_AND_READ_HPP

#include <boost/algorithm/string/predicate.hpp>
#include "tbb/parallel_pipeline.h"
#include "time_measurer.hpp"
#include "custom_types.hpp"
#include "limits.hpp"
#include <filesystem>
#include <fstream>
#include <memory>
#include <deque>
#include <array>

namespace list_and_read {
    std::string read_file_fast(std::ifstream& fs_in, bool check_size);
}

#endif // _LIST_AND_READ_HPP