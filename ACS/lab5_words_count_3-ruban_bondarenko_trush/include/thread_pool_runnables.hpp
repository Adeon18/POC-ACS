#ifndef WORD_INDEXER_THREAD_POOL_RUNNABLES_HPP
#define WORD_INDEXER_THREAD_POOL_RUNNABLES_HPP

#include "word_count.hpp"
#include "time_measurer.hpp"
#include "list_and_read.hpp"
#include "parallel-merge.hpp"
#include "tbb/concurrent_queue.h"
#include "QThreadPool"

namespace tp_runnables {
    namespace fs = std::filesystem;
    using custom_types::emptyable_pair;
    using time_measurer::mt_time_summmator_t;
    typedef std::unordered_map<std::string, size_t> um;

    class index_files_from_deque_runnable_t : public QRunnable {
    public:
        index_files_from_deque_runnable_t(
                tbb::concurrent_bounded_queue<emptyable_pair<std::string, std::string>> *mt_d_file_contents,
                tbb::concurrent_hash_map<std::string, size_t> *global_map
        ): mt_d_file_contents (mt_d_file_contents), global_map(global_map) {}

        void run() override {
            word_count::index_files_from_deque(*mt_d_file_contents, *global_map);
        }

    private:
        tbb::concurrent_bounded_queue<emptyable_pair < std::string, std::string>> *mt_d_file_contents;
        tbb::concurrent_hash_map<std::string, size_t> *global_map;
    };

    class add_files_to_queue_runnable_t : public QRunnable {
    public:
        add_files_to_queue_runnable_t(tbb::concurrent_bounded_queue<fs::path> *mt_d_filenames,
                                      std::string *indir,
                                      time_measurer::mt_time_summmator_t *time_sum): mt_d_filenames(mt_d_filenames),
                                                                                      indir(indir),
                                                                                      time_sum(time_sum) {}

        void run() override {
            list_and_read::add_files_to_queue(*mt_d_filenames, *indir, *time_sum);
        }

    private:
        tbb::concurrent_bounded_queue<fs::path> *mt_d_filenames;
        std::string *indir;
        time_measurer::mt_time_summmator_t *time_sum;
    };

    class read_files_from_deque_runnable_t : public QRunnable {
    public:
        read_files_from_deque_runnable_t(tbb::concurrent_bounded_queue<fs::path> *mt_d_filenames,
                                         tbb::concurrent_bounded_queue<emptyable_pair < std::string, std::string>> *mt_d_file_contents,
                                         time_measurer::mt_time_summmator_t *time_sum
        ): mt_d_filenames (mt_d_filenames), mt_d_file_contents(mt_d_file_contents), time_sum(time_sum) {}

        void run() override {
            list_and_read::read_files_from_deque(*mt_d_filenames, *mt_d_file_contents, *time_sum);
        }

    private:
        tbb::concurrent_bounded_queue<fs::path> *mt_d_filenames;
        tbb::concurrent_bounded_queue<emptyable_pair < std::string, std::string>> *mt_d_file_contents;
        time_measurer::mt_time_summmator_t *time_sum;
    };
}

#endif //WORD_INDEXER_THREAD_POOL_RUNNABLES_HPP
