#pragma once
#include <type_traits>
#include <iterator>

template <typename T>
concept HasBeginEnd = requires(T t) {
    { t.begin() } -> std::same_as<decltype(t.end())>;
};