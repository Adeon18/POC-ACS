#include "parallel-merge.hpp"

namespace {
    template<typename K, typename V>
    void merge_into_first(std::unordered_map<K, V>& map_1, std::unordered_map<K, V>& map_2) {
        for (const auto & item: map_2) {
            map_1[item.first] += item.second;
        }
    }
}

namespace parallel_merge {
    void merge(mt_queue_t<um>& mt_d_indexes, mt_queue_multi_t<um>& mt_qm_indexes) {
        um local_map;
        while (true) {
            um new_map = mt_d_indexes.pop();
            if (new_map.empty()) break;
            merge_into_first(local_map, new_map);
        }
        mt_qm_indexes.push(local_map);
        mt_qm_indexes.remove_data_creator();

        while (true) {
            auto [map_1, map_2] = mt_qm_indexes.pop_pair();

            if (map_1.empty()) break;

            if (map_1.size() < map_2.size()) {
                merge_into_first(map_2, map_1);
                mt_qm_indexes.push(map_2);
            } else {
                merge_into_first(map_1, map_2);
                mt_qm_indexes.push(map_1);
            }
        }
    }
}