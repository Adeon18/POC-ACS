// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "word_count.hpp"
#include "toml_parser.hpp"
#include "custom_types.hpp"
#include "time_measurer.hpp"
#include "options_parser.hpp"
#include "thread_pool_runnables.hpp"

#include "QThreadPool"
#include "boost/locale.hpp"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_hash_map.h"

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

    mt_time_summmator_t filenames_time_sum;
    mt_time_summmator_t file_contents_time_sum;

    // Static cast because size has to be unsigned, but set_capacity takes signed long.
    tbb::concurrent_bounded_queue<fs::path> mt_d_filenames;
    mt_d_filenames.set_capacity(static_cast<long>(config.max_filename_capacity));

    tbb::concurrent_bounded_queue<emptyable_pair<std::string, std::string>> mt_d_file_contents;
    mt_d_file_contents.set_capacity(static_cast<long>(config.max_file_contents_capacity));

    tbb::concurrent_hash_map<std::string, size_t> global_map;

    boost::locale::localization_backend_manager lbm
            = boost::locale::localization_backend_manager::global();
    lbm.select("icu");

    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));

    {
        auto add_files_to_queue_runnable = new tp_runnables::add_files_to_queue_runnable_t{&mt_d_filenames,
                                                                                           &config.indir,
                                                                                           &filenames_time_sum};
        auto read_files_from_deque_runnable = new tp_runnables::read_files_from_deque_runnable_t{&mt_d_filenames,
                                                                                                 &mt_d_file_contents,
                                                                                                 &file_contents_time_sum};
        QThreadPool::globalInstance()->start(add_files_to_queue_runnable);
        QThreadPool::globalInstance()->start(read_files_from_deque_runnable);
    }

    for (size_t i = 0; i < config.indexing_threads; ++i) {
        auto index_files_from_deque_runnable = new tp_runnables::index_files_from_deque_runnable_t{&mt_d_file_contents, &global_map};
        QThreadPool::globalInstance()->start(index_files_from_deque_runnable);
    }

#ifdef DEQUE_STATS
    std::thread stats_thread{[](
            tbb::concurrent_bounded_queue<fs::path>& mt_d_filenames,
            tbb::concurrent_bounded_queue<emptyable_pair<std::string, std::string>>& mt_d_file_contents
        ){
        int sec = 0;
        while(true) {
            auto mt_d_filenames_size = mt_d_filenames.size();

            std::cout << (sec++) / 2 << " s. SIZE mt_d_file_contents (object count): " << mt_d_file_contents.size() <<
            " mt_d_filenames (object count): " << mt_d_filenames_size << "   \r";
            std::cout.flush();
            if (mt_d_file_contents.size() <= 1 && mt_d_filenames_size <= 1 && sec > 1) break;
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
        }
    }, std::ref(mt_d_filenames), std::ref(mt_d_file_contents)};
#endif

    QThreadPool::globalInstance()->waitForDone();


#ifdef DEQUE_STATS
    stats_thread.join();
#endif

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
