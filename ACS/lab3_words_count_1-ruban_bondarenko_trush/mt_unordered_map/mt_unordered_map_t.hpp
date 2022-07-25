// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef TEMPLATE_MT_UNORDERED_MAP_T_H
#define TEMPLATE_MT_UNORDERED_MAP_T_H

#include <unordered_map>
#include <iostream>
#include <mutex>
#include <iterator>

namespace mt_unordered_map {
    template<typename K, typename V>
    class mt_unordered_map_t{
    public:
        using iterator = typename std::unordered_map<K, V>::iterator;
        using const_iterator = typename std::unordered_map<K, V>::const_iterator;

        mt_unordered_map_t() = default;
        ~mt_unordered_map_t() = default;
        mt_unordered_map_t(const mt_unordered_map_t&) = delete;
        mt_unordered_map_t& operator=(const mt_unordered_map_t&) = delete;

        void insert(const std::pair<K, V> &pair);
        void erase(const K& key);
        void clear() noexcept;
        void merge(const std::unordered_map<K, V> &map);
        // These functions are NOT THREAD SAFE
        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        // Thread-safe, but it is size lol
        size_t size();

    private:
        std::unordered_map<K, V> unordered_map_m;
        std::mutex mux_m;
    };

    template<typename K, typename V>
    void mt_unordered_map_t<K, V>::insert(const std::pair<K, V> &pair) {
        std::lock_guard<std::mutex> lock(mux_m);
        unordered_map_m.insert(pair);
    #ifdef UNORDERED_MAP_DEBUG
        std::cout << "insert:: Size(after call, pairs): " <<  unordered_map_m.size() << std::endl;
    #endif
    }

    template<typename K, typename V>
    void mt_unordered_map_t<K, V>::erase(const K &key) {
        std::lock_guard<std::mutex> lock(mux_m);
        unordered_map_m.erase(key);
    #ifdef UNORDERED_MAP_DEBUG
        std::cout << "erase:: Size(after call, pairs): " <<  unordered_map_m.size() << "; Key: " << key << std::endl;
    #endif
    }

    template<typename K, typename V>
    void mt_unordered_map_t<K, V>::clear() noexcept {
        std::lock_guard<std::mutex> lock(mux_m);
        unordered_map_m.clear();
    #ifdef UNORDERED_MAP_DEBUG
        std::cout << "clear:: Size(after call, pairs): " <<  unordered_map_m.size() << std::endl;
    #endif
    }

    template<typename K, typename V>
    typename std::unordered_map<K, V>::iterator mt_unordered_map_t<K, V>::begin() noexcept {
        return unordered_map_m.begin();
    }

    template<typename K, typename V>
    typename std::unordered_map<K, V>::iterator mt_unordered_map_t<K, V>::end() noexcept {
        return unordered_map_m.end();
    }

    template<typename K, typename V>
    typename std::unordered_map<K, V>::const_iterator mt_unordered_map_t<K, V>::cbegin() const noexcept {
        return unordered_map_m.cbegin();
    }

    template<typename K, typename V>
    typename std::unordered_map<K, V>::const_iterator mt_unordered_map_t<K, V>::cend() const noexcept {
        return unordered_map_m.cend();
    }

    template<typename K, typename V>
    size_t mt_unordered_map_t<K, V>::size() {
        std::lock_guard<std::mutex> lock(mux_m);
        return unordered_map_m.size();
    }

    template<typename K, typename V>
    void mt_unordered_map_t<K, V>::merge(const std::unordered_map<K, V> &map) {
        std::lock_guard<std::mutex> lock(mux_m);

        for (const auto & item: map) {
            unordered_map_m[item.first] += item.second;
        }
    }
}

#endif //TEMPLATE_MT_UNORDERED_MAP_T_H
