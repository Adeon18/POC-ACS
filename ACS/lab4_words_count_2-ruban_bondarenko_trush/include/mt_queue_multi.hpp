// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _MT_QUEUE_MULTI_HPP
#define _MT_QUEUE_MULTI_HPP

#include <deque>
#include <mutex>
#include <atomic>
#include <utility>
#include <iostream>
#include <filesystem>
#include <type_traits>
#include <condition_variable>

#include "custom_traits.hpp"


namespace mt_queue_multi {
    using custom_traits::has_empty;

    template<class T>
    class mt_queue_multi_t {
    public:
        mt_queue_multi_t() = delete;
        mt_queue_multi_t(unsigned long data_creators_left);
        ~mt_queue_multi_t() = default;
        mt_queue_multi_t(const mt_queue_multi_t&) = delete;
        mt_queue_multi_t& operator=(const mt_queue_multi_t&) = delete;

        template<typename E>
        void push(E&& elem);
        std::pair<T, T> pop_pair();
        T get_result();
        void remove_data_creator();

        // This is for debug
        size_t size();

    private:
        std::deque<std::pair<T, T>> pair_deque{};
        std::mutex deque_mutex{}, pair_mutex{};
        std::pair<T, T> pair_m{};
        bool pair_has_first = false;
        std::condition_variable cv_new_pair{};
        std::atomic<unsigned long> data_creators_left;
    };

    template<class T> mt_queue_multi_t<T>::mt_queue_multi_t(unsigned long data_creators_left) : data_creators_left(data_creators_left) {
        static_assert(has_empty<T>::value, "T must have a bool type empty() method");
    }

    template<class T>
    void mt_queue_multi_t<T>::remove_data_creator() {
        if (data_creators_left.fetch_sub(1, std::memory_order_release) == 1)
            cv_new_pair.notify_all();
    }

    template<class T>
    template<typename E>
    void mt_queue_multi_t<T>::push(E&& elem) {
        std::unique_lock<std::mutex> pair_lock(pair_mutex);
        if (pair_has_first) {
            pair_m.second = std::forward<E>(elem);
            {
                std::lock_guard<std::mutex> deque_lock(deque_mutex);
                pair_deque.push_front(std::move(pair_m));
            }
            pair_m = std::pair<T, T>{};
            pair_has_first = false;
            pair_lock.unlock();
            cv_new_pair.notify_one();
        } else {
            pair_m.first = std::forward<E>(elem);
            pair_has_first = true;
        }
    }

    template<class T>
    std::pair<T, T> mt_queue_multi_t<T>::pop_pair() {
        std::unique_lock<std::mutex> deque_lock(deque_mutex);

        while (pair_deque.empty()) {
            if (data_creators_left.load(std::memory_order_consume) == 0) {
                return std::pair<T, T>{};
            }
            cv_new_pair.wait(deque_lock);
        }

        std::pair<T, T> ans = std::move(pair_deque.back());
        pair_deque.pop_back();
        return ans;
    }

    template<class T>
    T mt_queue_multi_t<T>::get_result() {
        std::lock_guard<std::mutex> pair_lock(pair_mutex);
        if (data_creators_left.load(std::memory_order_consume) != 0 || !pair_has_first) return T();
        return std::move(pair_m.first);
    }

    template<class T>
    size_t mt_queue_multi_t<T>::size() {
        std::lock_guard<std::mutex> deque_lock(deque_mutex);
        return pair_deque.size();
    }
}

#endif //_MT_QUEUE_MULTI_HPP
