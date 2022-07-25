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

    class void_emptyable_function {
    public:
        void_emptyable_function() {
            function = nullptr;
        }
        void_emptyable_function(std::function<void()> function) : function(function) {}
        // void_emptyable_function(std::function<void()>&& function) : function(std::move(function)) {}

        template <typename ...Args>
        void operator()(Args&& ...args) const {
            function(std::forward<Args>(args)...);
        }

        bool empty() const {
            return function == nullptr;
        }
    private:
        std::function<void()> function;
    };
}

#endif  // CUSTOM_TYPES_HPP