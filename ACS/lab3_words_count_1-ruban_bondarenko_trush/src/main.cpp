// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <string>
#include <thread>
#include <iostream>
#include <filesystem>
#include "../mt_deque/mt_deque.hpp"
#include "../word_count/word_count.hpp"
#include "../toml_parser/toml_parser.hpp"
#include "../time_measurer/time_measurer.hpp"
#include "../list_and_read/list_and_read.hpp"
#include "../options_parser/options_parser.hpp"
#include "../mt_unordered_map/mt_unordered_map_t.hpp"

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

    time_measurer::mt_time_summmator_t filenames_time_sum;
    mt_deque::mt_deque_t<std::filesystem::path> mt_d_filenames;
    time_measurer::mt_time_summmator_t file_contents_time_sum;
    mt_deque::mt_deque_t<std::string> mt_d_file_contents;
    mt_unordered_map::mt_unordered_map_t<std::string, size_t> global_map;

    std::thread file_read_thread(list_and_read::read_files_from_deque, std::ref(mt_d_filenames), std::ref(mt_d_file_contents), std::ref(file_contents_time_sum));
    std::thread file_list_thread(list_and_read::add_files_to_queue, std::ref(mt_d_filenames), config.indir, std::ref(filenames_time_sum));

    std::vector<std::thread> file_index_thread_v;
    for (size_t i = 0; i < config.indexing_threads; i++) {
        file_index_thread_v.emplace_back(word_count::index_files_from_deque, std::ref(mt_d_file_contents), std::ref(global_map));
    }

    file_list_thread.join();
    file_read_thread.join();

    for (size_t i = 0; i < config.indexing_threads; i++) {
        file_index_thread_v[i].join();
    }

    auto test_start_write = time_measurer::get_current_time_fenced();
    word_count::write_map_sorted_by_key(global_map, config.out_by_a);
    word_count::write_map_sorted_by_value(global_map, config.out_by_n);
    auto test_time_write = time_measurer::get_current_time_fenced() - test_start_write;

    auto test_time_whole = time_measurer::get_current_time_fenced() - test_start_whole;

    std::cout << "Total=" << time_measurer::to_ms(test_time_whole) << std::endl;
    std::cout << "Reading=" << time_measurer::to_ms(file_contents_time_sum.total_time) << std::endl;
    std::cout << "Finding=" << time_measurer::to_ms(filenames_time_sum.total_time) << std::endl;
    std::cout << "Writing=" << time_measurer::to_ms(test_time_write) << std::endl;

	return 0;
}
