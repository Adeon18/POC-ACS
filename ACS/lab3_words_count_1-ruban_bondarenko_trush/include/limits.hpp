#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <cstdlib>

namespace size_limits {
    constexpr size_t KILOBYTE = 1000;
    constexpr size_t MEGABYTE = 1000 * KILOBYTE;
    constexpr size_t FILE_SIZE_LIMIT_BYTES = 10 * MEGABYTE;
    constexpr size_t DEQUE_SIZE_LIMIT_BYTES = 100 * MEGABYTE;
}

#endif  // LIMITS_HPP