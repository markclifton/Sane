#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "sane/ecs/components/position.hpp"
#include "sane/ecs/systems/system.hpp"
#include "sane/graphics/buffer.hpp"
#include "sane/graphics/shaderprogram.hpp"

namespace Sane
{
    namespace Systems
    {
        class Renderer2d : public System
        {
        public:
            Renderer2d(entt::registry& registry);
            virtual void Update(uint64_t ts) override;

        private:
            ShaderProgram sProg_;
            Buffer vbo_;
            Buffer ibo_;
            VertexAttrib vPos;
            VertexAttrib vCol;

            GLuint mvp_location;

            glm::mat4 proj_;
        };
    }
}