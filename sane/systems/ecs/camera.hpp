#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "sane/ecs/common.hpp"
#include "sane/events/common.hpp"
#include "sane/systems/ecs/base.hpp"
#include "sane/graphics/common.hpp"

namespace Sane
{
    namespace ECS
    {
        class Camera : public SystemBase, public Events::Listener
        {
            ShaderProgram sProg;
            Buffer vertices_buffer;
            Buffer normals_buffer;
            Buffer uvs_buffer;
            Buffer indices_buffer;
            VertexAttrib vPos;
            VertexAttrib vUV;

            float x{ 0 }, z{ 0 };
            float pitch{ 0 }, yaw{ -90.f };

            bool enableMouseMovement{ false };
            bool firstMovement{ false };
            Input::MouseMoveEvent lastMousePosition;
            Input::MouseMoveEvent nextMousePosition;

        public:
            Camera(entt::registry& registry);

            virtual bool ProcessEvent(Event& event) override;
            virtual void Update(double ts) override;
            virtual void RenderScene(glm::mat4 mvp) override;

            virtual void OnAttach() override;
        };
    }
}
