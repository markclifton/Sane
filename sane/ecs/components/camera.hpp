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
            glm::vec3 front{ 0.f, 0.f, 0.f };
            float velocity{ 1.f };
        };
    }
}