#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace Sane
{
    namespace Components
    {
        struct Position
        {
            Position(float x, float y, float z, float w)
                : data(x, y, z, w)
            {}
            Position(glm::vec4 data)
                : data(data)
            {}
            Position()
                : data(0, 0, 0, 0)
            {}
            glm::vec4 data;
        };
    }
}