// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef TEMPLATE_MT_DEQUE_HPP
#define TEMPLATE_MT_DEQUE_HPP

#include <deque>
#include <mutex>
#include <iostream>
#include <filesystem>
#include <condition_variable>
#include "../include/limits.hpp"

namespace mt_deque {
    template<typename T>
    size_t get_T_size(T v) {
        if constexpr (std::is_same_v<std::string, T>) {
            return v.length();
        } else if constexpr (std::is_same_v<std::filesystem::path, T>) {
            return sizeof(v) + v.string().length();
        } else {
            return sizeof(v);
        }
    }

    template<typename T>
    class mt_deque_t {
    public:
        mt_deque_t() = default;
        ~mt_deque_t() = default;
        mt_deque_t(const mt_deque_t&) = delete;
        mt_deque_t& operator=(const mt_deque_t&) = delete;

        void push_back(const T &elem);
        void push_front(const T& elem);
        T pop_front();
        T pop_back();
        // This is for debug
        size_t size();

    private:
        std::deque<T> deque_m;
        std::mutex mux_m;
        std::condition_variable cv_empty_m, cv_full_m;
    };

    template<typename T>
    void mt_deque_t<T>::push_back(const T &elem) {
        {
            std::unique_lock<std::mutex> lock(mux_m);
    #ifdef DEQUE_DEBUG
            std::cout << "push_back: Deque size(bytes): " << deque_m.size() * get_T_size(elem) << " Max size(bytes): " << size_limits::DEQUE_SIZE_LIMIT_BYTES << std::endl;
    #endif
            while (deque_m.size() * get_T_size(elem) >= size_limits::DEQUE_SIZE_LIMIT_BYTES) {
                cv_full_m.wait(lock);
            }
            deque_m.push_back(elem);
        }
        cv_empty_m.notify_one();
    }

    template<typename T>
    void mt_deque_t<T>::push_front(const T &elem) {
        {
            std::unique_lock<std::mutex> lock(mux_m);
    #ifdef DEQUE_DEBUG
            std::cout << "push_front: Deque size(bytes): " << deque_m.size() * get_T_size(elem) << " Max size(bytes): " << size_limits::DEQUE_SIZE_LIMIT_BYTES << std::endl;
    #endif
            while (deque_m.size() * get_T_size(elem) >= size_limits::DEQUE_SIZE_LIMIT_BYTES) {
                cv_full_m.wait(lock);
            }
            deque_m.push_front(elem);
        }
        cv_empty_m.notify_one();
    }

    template<typename T>
    T mt_deque_t<T>::pop_front() {
        T front_el;
        {
            std::unique_lock<std::mutex> lock(mux_m);

            while (deque_m.empty()) {
                cv_empty_m.wait(lock);
            }
            front_el = std::move(deque_m.front());
            deque_m.pop_front();
    #ifdef DEQUE_DEBUG
            std::cout << "pop_front: Deque size(bytes): " << deque_m.size() * get_T_size(front_el) << " Max size(bytes): " << size_limits::DEQUE_SIZE_LIMIT_BYTES << std::endl;
    #endif
        }
        cv_full_m.notify_one();

        return front_el;
    }

    template<typename T>
    T mt_deque_t<T>::pop_back() {
        T back_el;
        {
            std::unique_lock<std::mutex> lock(mux_m);

            while (deque_m.empty()) {
                cv_empty_m.wait(lock);
            }
            back_el = std::move(deque_m.back());
            deque_m.pop_back();
    #ifdef DEQUE_DEBUG
            std::cout << "pop_back: Deque size(bytes): " << deque_m.size() * get_T_size(front_el) << " Max size(bytes): " << size_limits::DEQUE_SIZE_LIMIT_BYTES << std::endl;
    #endif
        }
        cv_full_m.notify_one();

        return back_el;
    }

    template<typename T>
    size_t mt_deque_t<T>::size() {
        std::lock_guard<std::mutex> lock(mux_m);
        return deque_m.size();
    }
}

#endif //TEMPLATE_MT_DEQUE_HPP
