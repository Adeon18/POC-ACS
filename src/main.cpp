// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <tuple>
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
#include "tbb/flow_graph.h"
#include "tbb/tbb_allocator.h"
#include "tbb/parallel_pipeline.h"
#include "tbb/concurrent_hash_map.h"

namespace fs = std::filesystem;
using custom_types::emptyable_pair;
using time_measurer::mt_time_summmator_t;
using ss_empt_pair = custom_types::emptyable_pair<std::string, std::string>;
using path_continue_tuple = std::tuple<std::shared_ptr<fs::path>, tbb::flow::continue_msg>;
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

    tbb::flow::graph graph;

    tbb::flow::input_node<std::shared_ptr<fs::path>> file_lister_node(graph,
        list_and_read::file_lister(fs::recursive_directory_iterator(config.indir))
    );

    // when file_lister_limiter rejects, file_lister_node automatically pauses and buffers
    tbb::flow::limiter_node<std::shared_ptr<fs::path>> file_lister_limiter(graph, config.filenames_queue_max_size);

    // this join node is used to suspend execution of file_reader_node when raw files limit is met
    tbb::flow::join_node<path_continue_tuple, tbb::flow::queueing> pass_if_readlim_not_full(graph);

    tbb::flow::function_node<path_continue_tuple, std::shared_ptr<ss_empt_pair>, tbb::flow::queueing> file_reader_node(
        graph, 1,
        [&](const path_continue_tuple& path_ptr_tpl) -> std::shared_ptr<ss_empt_pair> {
            file_lister_limiter.decrementer().try_put(tbb::flow::continue_msg());

            fs::path file_path = *(std::get<0>(path_ptr_tpl).get());

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
                return  std::make_shared<ss_empt_pair>(
                    file_contents_str, file_path.extension().string()
                );
            }
            return std::make_shared<ss_empt_pair>();
        }
    );

    tbb::flow::limiter_node<std::shared_ptr<ss_empt_pair>> file_reader_limiter(graph, config.raw_files_queue_size);

    tbb::flow::function_node<std::shared_ptr<ss_empt_pair>, tbb::flow::continue_msg, tbb::flow::queueing> indexer_node(
        graph, config.indexing_threads,
        [&](std::shared_ptr<ss_empt_pair> ss_empt_pair_ptr) {
            tbb::flow::input_port<1>(pass_if_readlim_not_full).try_put(tbb::flow::continue_msg());

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
    );

    tbb::flow::make_edge(file_lister_node, file_lister_limiter);
    tbb::flow::make_edge(file_lister_limiter, tbb::flow::input_port<0>(pass_if_readlim_not_full));
    tbb::flow::make_edge(pass_if_readlim_not_full, file_reader_node); // file_reader_node decrements file_lister_limiter
    tbb::flow::make_edge(file_reader_node, indexer_node); // indexer_node adds tokens to pass_if_readlim_not_full

    // add initial amount of tokens
    for (unsigned long i = 0; i < config.raw_files_queue_size; ++i) {
        tbb::flow::input_port<1>(pass_if_readlim_not_full).try_put(tbb::flow::continue_msg());
    }

    file_lister_node.activate();
    graph.wait_for_all();

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
