#pragma once

#include "sane/graphics/common.hpp"
#include "sane/systems/ecs/base.hpp"

namespace Sane
{
    namespace ECS
    {
        class Grid : public SystemBase
        {
            ShaderProgram sProg;
            Buffer vertices_buffer;
            VertexAttrib vPos;

        public:
            Grid(entt::registry& registry);
            virtual void RenderScene(glm::mat4 mvp) override;
            virtual void OnAttach() override;
        };
    }
}