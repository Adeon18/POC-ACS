#ifndef CUSTOM_TRAITS_HPP
#define CUSTOM_TRAITS_HPP

namespace custom_traits {
    template<typename C>
    struct has_empty {
    private:
        template<typename T>
        static constexpr auto check(T*)
        -> typename std::is_same<decltype(std::declval<T>().empty()), bool>::type;

        template<typename>  // catch-all for types w/o empty()
        static constexpr std::false_type check(...);

        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };
}

#endif  // CUSTOM_TRAITS_HPP