#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

namespace Sane
{
    namespace Math
    {
        typedef glm::vec2 Vec2f;
        typedef glm::vec3 Vec3f;

        struct Vec2i
        {
            Vec2i(int32_t x, int32_t y)
                : x(x)
                , y(y)
            {}

            Vec2i(ImVec2 data)
                : x(int32_t(data.x))
                , y(int32_t(data.y))
            {
            }

            Vec2i(glm::vec2 data)
                : x(int32_t(data.x))
                , y(int32_t(data.y))
            {
            }

            int32_t x, y;
            operator glm::vec2() { return *(glm::vec2*)this; }
            operator ImVec2() { return *(ImVec2*)this; }
        };
    }
}