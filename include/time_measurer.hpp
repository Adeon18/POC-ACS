// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _TIME_MEASURER_HPP
#define _TIME_MEASURER_HPP

#include <mutex>
#include <chrono>
#include <atomic>
#include "time_measurer.hpp"

namespace time_measurer {
    inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
    {
        std::atomic_thread_fence(std::memory_order_seq_cst);
        auto res_time = std::chrono::high_resolution_clock::now();
        std::atomic_thread_fence(std::memory_order_seq_cst);
        return res_time;
    }

    template<class D>
    inline long long to_us(const D& d)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
    }

    template<class D>
    inline long long to_ms(const D& d)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
    }

    class mt_time_summmator_t {
    public:
        mt_time_summmator_t() {
            total_time = std::chrono::duration<long long, std::nano>(0);
        };
        ~mt_time_summmator_t() = default;
        mt_time_summmator_t(const mt_time_summmator_t&) = delete;
        mt_time_summmator_t& operator=(const mt_time_summmator_t&) = delete;

        std::chrono::duration<long long, std::nano> total_time;
        void add_time(const std::chrono::duration<long long, std::nano> &time) {
            std::unique_lock<std::mutex> lock(total_time_m);
            total_time += time;
        }
    private:
        std::mutex total_time_m;
    };

    template<typename T, typename ...Args>
    void timed_wrapper(mt_time_summmator_t &time_sum, T f, Args... args) {
        auto measure_start = get_current_time_fenced();
        f(args...);
        auto total_time = get_current_time_fenced() - measure_start;
        time_sum.add_time(total_time);
    }
}

#endif  // _TIME_MEASURER_HPP
