#include "list_and_read.hpp"
#include <iostream>

namespace list_and_read {
    std::string read_file_fast(std::ifstream& fs_in, bool check_size) {
        auto const start_pos = fs_in.tellg();
        fs_in.ignore(std::numeric_limits<std::streamsize>::max());
        auto const char_count = fs_in.gcount();
        if (check_size && char_count > static_cast<std::streamsize>(size_limits::FILE_SIZE_LIMIT_BYTES))
            return "";
        fs_in.seekg(start_pos);
        auto s = std::string(char_count, char{});
        fs_in.read(&s[0], s.size());
        return s;
    }
    
    std::pair<std::string, std::string> read_file_by_name(const fs::path &file_path) {
        std::ifstream fs_in;
        fs_in.open(file_path, std::ifstream::in | std::ifstream::binary);

        if (!(fs_in.is_open())) {
            // ignore unopenable files
            std::cerr << "list_and_read::read_file_by_name::Could not open file" << std::endl;
        }

        bool check_size = file_path.extension() == ".txt";
        std::string file_contents_str = read_file_fast(fs_in, check_size);
        fs_in.close();

        return std::pair(file_contents_str, file_path.extension());
    }
}