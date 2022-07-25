#include "list_and_read.hpp"

namespace list_and_read {
    void add_files_to_queue(
        mt_queue::mt_queue_t<fs::path>& mt_d_filenames, const std::string &indir,
        time_measurer::mt_time_summmator_t &time_sum
    ) {
        auto measure_start = time_measurer::get_current_time_fenced();
        for (auto const& dir_entry : fs::recursive_directory_iterator(indir)) {
            if (!dir_entry.path().empty()) {
                if (dir_entry.path().extension() == ".txt" || dir_entry.path().extension() == ".zip") {
                    mt_d_filenames.push(dir_entry.path());
                }
            }
        }
        time_sum.add_time(time_measurer::get_current_time_fenced() - measure_start);
        mt_d_filenames.remove_data_creator();
    }

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

    void read_files_from_deque(
        mt_queue::mt_queue_t<fs::path>& mt_d_filenames, mt_queue::mt_queue_t<emptyable_pair<std::string, std::string>>& mt_d_file_contents,
        time_measurer::mt_time_summmator_t &time_sum
    ) {
        while (true) {
            auto file_path = mt_d_filenames.pop();

            auto measure_start = time_measurer::get_current_time_fenced();

            [[unlikely]] if (file_path.empty()) {
                break;
            }

            std::ifstream fs_in;
            fs_in.open(file_path, std::ifstream::in | std::ifstream::binary);
            if (!(fs_in.is_open())) {
                // ignore unopenable files
                continue;
            }

            bool check_size = file_path.extension() == ".txt";
            std::string file_contents_str = read_file_fast(fs_in, check_size);
            fs_in.close();

            // Compromise: intoducing error, but not counting the stalls.
            time_sum.add_time(time_measurer::get_current_time_fenced() - measure_start);

            if (!file_contents_str.empty())
                mt_d_file_contents.push(emptyable_pair<std::string, std::string>(file_contents_str, file_path.extension()));
        }
        mt_d_file_contents.remove_data_creator();
    }
}