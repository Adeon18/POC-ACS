// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <queue>
#include <string>
#include <chrono>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "merge.hpp"
#include "word_count.hpp"
#include "toml_parser.hpp"
#include "custom_types.hpp"
#include "time_measurer.hpp"
#include "options_parser.hpp"
#include "archive_manager.hpp"

#include "QFuture"
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentMap>
#include "boost/locale.hpp"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_hash_map.h"
#include "list_and_read.hpp"

namespace fs = std::filesystem;
using custom_types::emptyable_pair;
using time_measurer::mt_time_summmator_t;
typedef std::unordered_map<std::string, size_t> um;



int main(int argc, char* argv[]) {
    command_line_options_t command_line_options{argc, argv};

    toml_parser::config_t config;
    switch (read_config(command_line_options.get_config_file(), config)) {
    case toml_parser::STATUS_SYNTAX_ERROR:
        exit(EXIT_FAILURE);
        break;
    case toml_parser::STATUS_OPEN_ERROR:
        std::cout << "Config file open error" << std::endl;
        exit(EXIT_FAILURE);
        break;
    default:
        break;
    }

    auto test_start_whole = time_measurer::get_current_time_fenced();

    QVector<std::pair<std::string, std::string>> mt_d_filenames;

    boost::locale::localization_backend_manager lbm
            = boost::locale::localization_backend_manager::global();
    lbm.select("icu");

    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));


    for (auto const& dir_entry : fs::recursive_directory_iterator(config.indir)) {
        if (!dir_entry.path().empty()) {
            if (dir_entry.path().extension() == ".txt" || dir_entry.path().extension() == ".zip") {
                mt_d_filenames.append(list_and_read::read_file_by_name(dir_entry.path()));
            }
        }
    }

    QFuture<std::unordered_map<std::string, size_t>> result = QtConcurrent::mappedReduced(
            mt_d_filenames,
            word_count::unzip_index_and_get_map,
            merge::merge_dictionaries
            );

    result.waitForFinished();

    auto global_map = result.result();

    auto test_time_whole = time_measurer::get_current_time_fenced() - test_start_whole;

    auto test_start_write = time_measurer::get_current_time_fenced();
    word_count::write_map_sorted_by_key(global_map, config.out_by_a);
    word_count::write_map_sorted_by_value(global_map, config.out_by_n);
    auto test_time_write = time_measurer::get_current_time_fenced() - test_start_write;

    std::cout << "Total=" << time_measurer::to_ms(test_time_whole) << std::endl;
//    std::cout << "Total=" << time_measurer::to_ms(test_time_whole) << std::endl;
//    std::cout << "Total=" << time_measurer::to_ms(test_time_whole) << std::endl;
    std::cout << "Writing=" << time_measurer::to_ms(test_time_write) << std::endl;

    return 0;
}
