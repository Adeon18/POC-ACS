#include "list_and_read.hpp"

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
}