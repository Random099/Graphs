#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"

#include <type_traits>
#include <iterator>

template <typename T>
concept HasBeginEnd = requires(T t) {
    { t.begin() } -> std::same_as<decltype(t.end())>;
};

namespace ImGuiColors 
{
    namespace {
        ImU32 white = IM_COL32(255, 255, 255, 255);
        ImU32 red = IM_COL32(255, 0, 0, 255);
    }
}