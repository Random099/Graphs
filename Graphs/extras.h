#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"

#include <type_traits>
#include <iterator>
#include <cstdint>

template <typename T>
concept HasBeginEnd = requires(T t) {
    { t.begin() } -> std::same_as<decltype(t.end())>;
};

namespace ImGuiColors 
{
    namespace {
        constexpr ImU32 WHITE = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 RED = IM_COL32(255, 0, 0, 255);
        constexpr ImU32 YELLOW = IM_COL32(249, 180, 45, 255);
    }
}

namespace constants
{
    namespace
    {
        constexpr uint32_t POINT_SELECTOR_RADIUS = 15;
    }
}