#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"

#include <type_traits>
#include <iterator>
#include <cstdint>
#include <cmath>
#include <iostream>

template <typename T>
concept HasBeginEnd = requires(T t) {
    { t.begin() } -> std::same_as<decltype(t.end())>;
};

namespace ImGuiColors 
{
    namespace 
    {
        constexpr ImU32 WHITE = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 WHITE_TRANSPARENT = IM_COL32(255, 255, 255, 50);
        constexpr ImU32 RED = IM_COL32(255, 0, 0, 255);
        constexpr ImU32 YELLOW = IM_COL32(249, 230, 45, 255);
        constexpr ImU32 GREEN = IM_COL32(1, 200, 32, 255);
    }
}

namespace constant
{
    namespace
    {
        constexpr uint32_t POINT_SELECTOR_RADIUS{ 15 };
        constexpr float POINT_RADIUS{ 5.0f };
        constexpr float LINE_THICKNESS{ 2.0f };
        constexpr float WINDOW_X_MIN_OFFSET{ 5.0f };
        constexpr float WINDOW_X_MAX_OFFSET{ 20.0f };
        constexpr float WINDOW_Y_MIN_OFFSET{ 10.0f };
        constexpr float WINDOW_Y_MAX_OFFSET{ 60.0f };
        constexpr ImVec2 VERTEX_TEXT_OFFSET{ 3.5f, 20.0f };
        constexpr ImVec2 DEFAULT_GRAPH_WINDOW_SIZE{ 500, 540 };
    }
}

namespace helper
{
    namespace
    {
        inline ImVec2 MiddlePoint(const ImVec2& point1, const ImVec2& point2)
        {
            return ImVec2((point1.x + point2.x) / 2, (point1.y + point2.y) / 2);
        }

        template <typename T, typename U>
        inline T Distance(const U& point1, const U& point2)
        {
			return static_cast<T>(std::sqrt(std::pow(point2.x - point1.x, 2.0f) + std::pow(point2.y - point1.y, 2.0f)));
		}   
    }
}