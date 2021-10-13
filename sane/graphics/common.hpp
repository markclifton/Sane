#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "sane/graphics/buffer.hpp"
#include "sane/graphics/framebuffer.hpp"
#include "sane/graphics/shaderprogram.hpp"
#include "sane/graphics/texture.hpp"

namespace Sane
{
    namespace VertexData
    {
        struct Position
        {
            Position()
                : Position(0, 0, 0, 0)
            {}

            Position(float x, float y, float z, float w)
                : x(x), y(y), z(z), w(w)
            {}

            Position(float x, float y, float z)
                : Position(x, y, z, 1)
            {}

            float x, y, z, w;
        };

        struct Normal
        {
            float x, y, z;
        };

        struct UV
        {
            float u, v, w;
        };

        struct Color
        {
            float r, g, b, a;
        };
    }
}