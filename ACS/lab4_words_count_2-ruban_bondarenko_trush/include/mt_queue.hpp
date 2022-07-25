// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef TEMPLATE_MT_QUEUE_HPP
#define TEMPLATE_MT_QUEUE_HPP

#include <deque>
#include <mutex>
#include <utility>
#include <iostream>
#include <filesystem>
#include <condition_variable>

#include "limits.hpp"
#include "custom_types.hpp"
#include "custom_traits.hpp"

namespace mt_queue {
    using custom_traits::has_empty;

    template<typename T>
    size_t get_T_size(const T& v) {
        if constexpr (std::is_same_v<std::string, T>) {
            return v.length();
        } else if constexpr (std::is_same_v<std::filesystem::path, T>) {
            return sizeof(v) + v.string().length();
        } else if constexpr (std::is_same_v<std::pair<std::string, std::string>, T>) {
            return v.first.length() + v.second.length();
        } else if constexpr (std::is_same_v<custom_types::emptyable_pair<std::string, std::string>, T>) {
            return v.first.length() + v.second.length();
        } else {
            return sizeof(v);
        }
    }

    template<typename T>
    class mt_queue_t {
    public:
        mt_queue_t() = delete;
        mt_queue_t(unsigned long data_creators_left);
        ~mt_queue_t() = default;
        mt_queue_t(const mt_queue_t&) = delete;
        mt_queue_t& operator=(const mt_queue_t&) = delete;

        template<typename E>
        void push(E&& elem);
        T pop();
        void remove_data_creator();

        // WARNING: For debug/stats purposes only
        size_t size();
        // WARNING: For debug/stats purposes only
        unsigned long long get_deque_size_bytes();

    private:
        std::deque<T> deque_m{};
        std::mutex mux_m{};  // protects deque_m, deque_size_bytes.
        std::condition_variable cv_new_element{}, cv_space_available{};
        unsigned long long deque_size_bytes = 0;
        std::atomic<unsigned long> data_creators_left;
    };

    template<typename T>
    mt_queue_t<T>::mt_queue_t(unsigned long data_creators_left): data_creators_left(data_creators_left) {
        static_assert(has_empty<T>::value, "T must have a bool type empty() method");
    }

    template<typename T>
    template<typename E>
    void mt_queue_t<T>::push(E&& elem) {
        {
            std::unique_lock<std::mutex> lock(mux_m);
    #ifdef DEQUE_DEBUG
            std::cout << "push_back: Deque size(bytes): " << deque_size_bytes << " Max size(bytes): " << size_limits::DEQUE_SIZE_LIMIT_BYTES << std::endl;
    #endif
            while (deque_size_bytes >= size_limits::DEQUE_SIZE_LIMIT_BYTES) {
                cv_space_available.wait(lock);
            }
            deque_size_bytes += get_T_size(std::forward<E>(elem));
            deque_m.push_back(std::forward<E>(elem));
        }
        cv_new_element.notify_one();
    }

    template<typename T>
    T mt_queue_t<T>::pop() {
        T front_el;
        {
            std::unique_lock<std::mutex> lock(mux_m);

            while (deque_m.empty()) {
                if (data_creators_left.load(std::memory_order_consume) == 0) return T();
                cv_new_element.wait(lock);
            }
            front_el = std::move(deque_m.front());
            deque_size_bytes -= get_T_size(front_el);
            deque_m.pop_front();
    #ifdef DEQUE_DEBUG
            std::cout << "pop_front: Deque size(bytes): " << deque_size_bytes << " Max size(bytes): " << size_limits::DEQUE_SIZE_LIMIT_BYTES << std::endl;
    #endif
        }
        cv_space_available.notify_one();

        return front_el;
    }

    template<typename T>
    void mt_queue_t<T>::remove_data_creator() {
        if (data_creators_left.fetch_sub(1, std::memory_order_release) == 1)
            cv_new_element.notify_all();
    }

    template<typename T>
    unsigned long long mt_queue_t<T>::get_deque_size_bytes() {
        std::lock_guard<std::mutex> lock(mux_m);
        return deque_size_bytes;
    }

    template<typename T>
    size_t mt_queue_t<T>::size() {
        std::lock_guard<std::mutex> lock(mux_m);
        return deque_m.size();
    }
}

#endif //TEMPLATE_MT_QUEUE_HPP
