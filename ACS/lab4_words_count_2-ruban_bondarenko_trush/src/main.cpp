// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "mt_queue.hpp"
#include "word_count.hpp"
#include "toml_parser.hpp"
#include "thread_pool.hpp"
#include "custom_types.hpp"
#include "time_measurer.hpp"
#include "list_and_read.hpp"
#include "parallel-merge.hpp"
#include "options_parser.hpp"
#include "mt_queue_multi.hpp"
#include "boost/locale.hpp"

using mt_queue::mt_queue_t;
namespace fs = std::filesystem;
using thread_pool::thread_pool_t;
using custom_types::emptyable_pair;
using mt_queue_multi::mt_queue_multi_t;
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

    mt_time_summmator_t filenames_time_sum;
    mt_time_summmator_t file_contents_time_sum;

    mt_queue_t<fs::path> mt_d_filenames{1};
    mt_queue_t<emptyable_pair<std::string, std::string>> mt_d_file_contents{1};
    mt_queue_t<um> mt_d_indexes{config.indexing_threads};
    mt_queue_multi_t<um> mt_qm_indexes{config.merging_threads};  // config.indexing_threads

    boost::locale::localization_backend_manager lbm
            = boost::locale::localization_backend_manager::global();
    lbm.select("icu");

    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));

    thread_pool_t t_pool {config.indexing_threads + config.merging_threads + 2};
    t_pool.push_task(list_and_read::add_files_to_queue, std::ref(mt_d_filenames), config.indir, std::ref(filenames_time_sum));
    t_pool.push_task(list_and_read::read_files_from_deque, std::ref(mt_d_filenames), std::ref(mt_d_file_contents), std::ref(file_contents_time_sum));

    for (size_t i = 0; i < config.indexing_threads; ++i) {
        t_pool.push_task(word_count::index_files_from_deque, std::ref(mt_d_file_contents), std::ref(mt_d_indexes));
    }

    for (size_t i = 0; i < config.merging_threads; ++i) {
        t_pool.push_task(parallel_merge::merge, std::ref(mt_d_indexes), std::ref(mt_qm_indexes));
    }

#ifdef DEQUE_STATS
    std::thread stats_thread{[](
            mt_queue_t<fs::path>& mt_d_filenames,
            mt_queue::mt_queue_t<emptyable_pair<std::string, std::string>>& mt_d_file_contents,
            mt_queue_t<um>& mt_d_indexes, mt_queue_multi_t<um>& mt_qm_indexes
        ){
        int sec = 0;
        while(true) {
            auto mt_d_filenames_size = mt_d_filenames.size();
            auto mt_d_indexes_size = mt_d_indexes.size();
            auto mt_qm_indexes_size = mt_qm_indexes.size();
            std::cout << (sec++) / 2 << " s. SIZE mt_d_file_contents (bytes): " << mt_d_file_contents.get_deque_size_bytes() <<
            " mt_d_filenames (items): " << mt_d_filenames_size << " mt_d_indexes (items): " << mt_d_indexes_size <<
            " mt_qm_indexes (items): " << mt_qm_indexes_size << "   \r";
            std::cout.flush();
            if (mt_d_file_contents.size() <= 1 && mt_d_filenames_size <= 1 && mt_d_indexes_size <= 1 && mt_qm_indexes_size == 0 && sec > 1) break;
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
        }
    }, std::ref(mt_d_filenames), std::ref(mt_d_file_contents), std::ref(mt_d_indexes), std::ref(mt_qm_indexes)};
#endif

    t_pool.wait_for_tasks();

#ifdef DEQUE_STATS
    stats_thread.join();
#endif

    um global_map = mt_qm_indexes.get_result();

    auto test_time_whole = time_measurer::get_current_time_fenced() - test_start_whole;

    auto test_start_write = time_measurer::get_current_time_fenced();
    word_count::write_map_sorted_by_key(global_map, config.out_by_a);
    word_count::write_map_sorted_by_value(global_map, config.out_by_n);
    auto test_time_write = time_measurer::get_current_time_fenced() - test_start_write;

    std::cout << "Total=" << time_measurer::to_ms(test_time_whole) << std::endl;
    std::cout << "Reading=" << time_measurer::to_ms(file_contents_time_sum.total_time) << std::endl;
    std::cout << "Finding=" << time_measurer::to_ms(filenames_time_sum.total_time) << std::endl;
    std::cout << "Writing=" << time_measurer::to_ms(test_time_write) << std::endl;

    return 0;
}
