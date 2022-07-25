#ifndef WORD_INDEXER_THREAD_POOL_HPP
#define WORD_INDEXER_THREAD_POOL_HPP

#include <cstdio>
#include <atomic>
#include <thread>
#include <vector>
#include <functional>

// TODO: this is a bad queue choice
#include "mt_queue.hpp"
#include "custom_types.hpp"

namespace thread_pool {
    using custom_types::void_emptyable_function;

    class thread_pool_t {
    public:
        thread_pool_t(size_t thread_count_) : thread_count(thread_count_), tasks(1) {
            create_threads();
        }

        ~thread_pool_t() {
            tasks.remove_data_creator();
            wait_for_tasks();
            join_threads();
        }

        size_t get_tasks_running() {
            return tasks_count - tasks.size();
        }

        template<typename F, typename... Args>
        void push_task(const F &task, const Args &...args) {
            tasks.push(void_emptyable_function([task, args...] { task(args...); }));
            ++tasks_count;
        }

        void wait_for_tasks() {
            while (true) {
                if (!paused) {
                    if (tasks_count == 0) {
                        break;
                    }
                } else {
                    if (get_tasks_running() == 0) {
                        break;
                    }
                }
                sleep_or_yield();
            }
        }

        void pause() {
            paused = true;
        }

        void unpause() {
            paused = false;
        }

    private:
        void sleep_or_yield() {
            if (sleep_duration) {
                std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration));
            } else {
                std::this_thread::yield();
            }
        }

        void create_threads() {
            for (size_t i = 0; i < thread_count; ++i) {
                threads.emplace_back(&thread_pool_t::worker, this);
            }
        }

        void join_threads() {
            for (auto &thread: threads) {
                thread.join();
            }
        }

        void worker() {
            while (true) {
                if (!paused) {
                    auto task = tasks.pop();
                    if (task.empty()) {
                        break;
                    }
                    task();
                    --tasks_count;
                } else {
                    sleep_or_yield();
                }
            }
        }

        size_t thread_count = 1;
        size_t sleep_duration = 0;
        std::atomic<bool> paused = false;
        mt_queue::mt_queue_t<void_emptyable_function> tasks;
        std::atomic<std::uint64_t> tasks_count = 0;
        std::vector<std::thread> threads;
    };
}

#endif //WORD_INDEXER_THREAD_POOL_HPP
