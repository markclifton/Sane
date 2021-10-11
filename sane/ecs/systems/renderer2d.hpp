#pragma once

#include "sane/ecs/common.hpp"
#include "sane/graphics/common.hpp"

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