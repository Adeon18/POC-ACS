#ifndef CUSTOM_TYPES_HPP
#define CUSTOM_TYPES_HPP

#include <utility>
#include <functional>

namespace custom_types {
    template<typename T1, typename T2>
    struct emptyable_pair {
        T1 first{};
        T2 second{};
        emptyable_pair() = default;
        emptyable_pair(T1 first, T2 second) : first(first), second(second) {}
        emptyable_pair(T1&& first, T2&& second) : first(std::move(first)), second(std::move(second)) {}
        bool empty() const {
            return first.empty() && second.empty();
        }
    };
}

#endif  // CUSTOM_TYPES_HPP