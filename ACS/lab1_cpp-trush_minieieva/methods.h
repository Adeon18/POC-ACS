#ifndef TEMPLATE_METHODS_H
#define TEMPLATE_METHODS_H

enum Errors_codes {
    WRONG_NUM_ARGUMENTS = 1,
    WRONG_METHOD = 2,
    ERROR_OPENING_INPUT_FILE = 3,
    ERROR_OPENING_OUTPUT_FILE = 4,
    ERROR_READING_INPUT_FILE = 5,
    ERROR_WRITING_TO_OUTPUT_FILE = 6,
    NULLPTR_ERROR = 7,
};


template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}


size_t method1_easy_easy(std::ifstream &file, std::string *str_res);
size_t method2_ignore(std::ifstream &file, std::string *str_res);
size_t method3_chunk_read(std::ifstream &file, std::deque<char> *storage);
size_t method4_stream_iterators(std::ifstream &file, std::string *data);
std::pair<unsigned int, double> operate_on_data(std::string &data);
size_t write_data_to_file(const std::string &out_file_name, std::pair<unsigned int, double> results);
std::string deque_to_string(std::deque<char> &deque);

#endif //TEMPLATE_METHODS_H
