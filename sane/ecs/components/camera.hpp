#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace Sane
{
    namespace Components
    {
        struct Camera
        {
            bool enabled;

            glm::mat4 lookat{ 1.f };
        };
    }
}