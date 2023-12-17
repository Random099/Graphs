#pragma once
#include <type_traits>

template <typename T>
struct hasBeginEnd {
    template <typename U>
    static auto test(U* u) -> decltype(u->begin(), u->end(), std::true_type());

    template <typename U>
    static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
};