// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <deque>
#include <mutex>
#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <functional>
#include "merge.hpp"
#include "word_count.hpp"
#include "exit_codes.hpp"
#include "archive_manager.hpp"
#include <boost/algorithm/string.hpp>


namespace archive_manager {
    /*
    Unzip the file from memory and read the pass each txt fie to count_words for indexing.
    Reads only .txt and files < 10 MB.
    */
    std::unordered_map<std::string, size_t> unzip_and_index(const std::string &archive, char* buffer) {

        struct archive *arch;
        struct archive_entry *entry = nullptr;
        int err_code;

        std::unordered_map<std::string, size_t> local_map;

        arch = archive_read_new();
        archive_read_support_filter_all(arch);
        archive_read_support_format_zip(arch);

        err_code = archive_read_open_memory(arch, archive.data(), archive.size());

        if (err_code != ARCHIVE_OK) {
#ifdef ARCHIVE_DEBUG
            std::cerr << "Error Opening archive" << std::endl;
#endif
        } else {
            while (archive_read_next_header(arch, &entry) == ARCHIVE_OK) {
                std::filesystem::path file_path(archive_entry_pathname(entry));
                auto file_size = archive_entry_size(entry);
                auto size_lim = static_cast<decltype(file_size)>(size_limits::FILE_SIZE_LIMIT_BYTES);

                if (
                    file_path.extension().string() == ".txt" && file_size > 0 && file_size < size_lim
                ) {
                    auto read_len = archive_read_data(arch, buffer, file_size);
                    if (read_len >= 0 && read_len < size_lim) {
                        auto entry_map = word_count::count_words(buffer, buffer + read_len);
                        merge::merge_dictionaries(local_map, entry_map);
                    }
                }
            }
        }

        err_code = archive_read_free(arch);

        if (err_code != ARCHIVE_OK) {
#ifdef ARCHIVE_DEBUG
            std::cerr << "Error Closing archive" << std::endl;
#endif
        }

        return local_map;
    }
}