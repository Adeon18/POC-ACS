#include <deque>
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
#include "list_and_read.hpp"
#include "options_parser.hpp"
#include "archive_manager.hpp"

#include "boost/locale.hpp"
#include "folly/futures/Future.h"
#include "folly/executors/CPUThreadPoolExecutor.h"

using time_measurer::mt_time_summmator_t;

int main(int argc, char** argv) {
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

    std::unordered_map<std::string, size_t> global_map;

    boost::locale::localization_backend_manager lbm
            = boost::locale::localization_backend_manager::global();
    lbm.select("icu");

    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));

    std::vector<folly::Future<std::unordered_map<std::string, size_t>>> futures;
    folly::CPUThreadPoolExecutor cpu_thread_pool_executor{config.indexing_threads};

    auto test_start_whole = time_measurer::get_current_time_fenced();

    auto test_start_filenames = time_measurer::get_current_time_fenced();
    for (const auto & dir_entry: std::filesystem::recursive_directory_iterator(config.indir)) {
        if (!dir_entry.path().empty()) {
            if (dir_entry.path().extension() == ".txt" || dir_entry.path().extension() == ".zip") {
                futures.emplace_back(
                        folly::makeFuture(list_and_read::read_file_by_name(dir_entry.path()))
                                .via(&cpu_thread_pool_executor)
                                .thenValue([&](const std::pair<std::string, std::string>& file_contents) {
                                    std::unordered_map<std::string, size_t> result;

                                    if (file_contents.second == ".txt") {
                                        result = word_count::count_words(
                                                file_contents.first.cbegin(), file_contents.first.cend()
                                        );
                                    } else if (file_contents.second == ".zip") {
                                        char* buffer = new char[size_limits::FILE_SIZE_LIMIT_BYTES];
                                        result = archive_manager::unzip_and_index(file_contents.first, buffer);
                                        delete [] buffer;
                                    }

                                    return result;
                                })
                );
            }
        }
    }

    auto test_time_filenames = time_measurer::get_current_time_fenced() - test_start_filenames;

    auto res = folly::unorderedReduce(futures.begin(),
                                                      futures.end(),
                                                      std::unordered_map<std::string, size_t> {},
                                                      merge::merge_dictionaries).via(&cpu_thread_pool_executor).get();

    auto test_time_whole = time_measurer::get_current_time_fenced() - test_start_whole;

    auto test_start_write = time_measurer::get_current_time_fenced();
    word_count::write_map_sorted_by_key(res, config.out_by_a);
    word_count::write_map_sorted_by_value(res, config.out_by_n);
    auto test_time_write = time_measurer::get_current_time_fenced() - test_start_write;


    std::cout << "Total=" << time_measurer::to_ms(test_time_whole) << std::endl;
    std::cout << "Finding=" << time_measurer::to_ms(test_time_filenames) << std::endl;
    std::cout << "Writing=" << time_measurer::to_ms(test_time_write) << std::endl;

    return 0;
}

