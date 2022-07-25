// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include "word_count.hpp"
#include "toml_parser.hpp"
#include "custom_types.hpp"
#include "time_measurer.hpp"
#include "list_and_read.hpp"
#include "options_parser.hpp"
#include "archive_manager.hpp"

#include "boost/locale.hpp"
#include "tbb/tbb_allocator.h"
#include "tbb/parallel_pipeline.h"
#include "tbb/concurrent_hash_map.h"

namespace fs = std::filesystem;
using custom_types::emptyable_pair;
using time_measurer::mt_time_summmator_t;
using ss_empt_pair = custom_types::emptyable_pair<std::string, std::string>;
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

    tbb::concurrent_hash_map<std::string, size_t> global_map;

    boost::locale::localization_backend_manager lbm
            = boost::locale::localization_backend_manager::global();
    lbm.select("icu");

    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));

    auto filenames_iter = fs::recursive_directory_iterator(config.indir);
    auto filenames_begin = fs::begin(filenames_iter);
    auto filenames_end = fs::end(filenames_iter);

    tbb::parallel_pipeline(
        config.max_threads,
        tbb::make_filter<void, std::shared_ptr<fs::path>>(
            tbb::filter_mode::serial_out_of_order,
            [&](tbb::flow_control& fc) -> std::shared_ptr<fs::path> {
                if (filenames_begin == filenames_end) {
                    fc.stop();
                    return std::make_shared<fs::path>();
                }
                auto dir_entry = *(filenames_begin++);
                if (!dir_entry.path().empty()) {
                    if (dir_entry.path().extension() == ".txt" || dir_entry.path().extension() == ".zip") {
                        return std::make_shared<fs::path>(std::move(dir_entry.path()));
                    }
                }
                return std::make_shared<fs::path>();
            }) &
        tbb::make_filter<std::shared_ptr<fs::path>, std::shared_ptr<ss_empt_pair>>(
            tbb::filter_mode::serial_out_of_order,
            [&](std::shared_ptr<fs::path> path_ptr) -> std::shared_ptr<ss_empt_pair> {
                fs::path file_path = *(path_ptr.get());

                if (file_path.empty()) {
                    return std::make_shared<ss_empt_pair>();
                }

                auto measure_start = time_measurer::get_current_time_fenced();

                std::ifstream fs_in;
                fs_in.open(file_path, std::ifstream::in | std::ifstream::binary);
                if (!(fs_in.is_open())) {
                    // ignore unopenable files
                    return std::make_shared<ss_empt_pair>();
                }

                bool check_size = file_path.extension().string() == ".txt";
                std::string file_contents_str = list_and_read::read_file_fast(fs_in, check_size);
                fs_in.close();

                // Compromise: intoducing error, but not counting the stalls.
                file_contents_time_sum.add_time(time_measurer::get_current_time_fenced() - measure_start);

                if (!file_contents_str.empty()) {
                    return std::make_shared<ss_empt_pair>(file_contents_str, file_path.extension().string());
                }
                return std::make_shared<ss_empt_pair>();
            }) &
        tbb::make_filter<std::shared_ptr<ss_empt_pair>, void>(
            tbb::filter_mode::parallel,
            [&](std::shared_ptr<ss_empt_pair> ss_empt_pair_ptr) {
                auto file_contents = *(ss_empt_pair_ptr.get());
                if (file_contents.empty()) {
                    return;
                }

                if (file_contents.second == ".txt") {
                    word_count::count_words(
                        file_contents.first.cbegin(), file_contents.first.cend(),
                        global_map
                    );
                } else if (file_contents.second == ".zip") {
                    char* buffer = tbb::tbb_allocator<char>().allocate(size_limits::FILE_SIZE_LIMIT_BYTES);
                    archive_manager::unzip_and_index(file_contents.first, global_map, buffer);
                    tbb::tbb_allocator<char>().deallocate(buffer, size_limits::FILE_SIZE_LIMIT_BYTES);
                }
            }
        )
    );

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
