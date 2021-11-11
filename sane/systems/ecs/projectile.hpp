#pragma once

#include "sane/graphics/common.hpp"
#include "sane/systems/ecs/base.hpp"
#include "sane/events/common.hpp"

namespace Sane
{
    namespace ECS
    {
        class Projectile : public SystemBase, public Events::Listener
        {
            ShaderProgram sProg;
            Buffer vertices_buffer;
            VertexAttrib vPos;
            bool heldMouse{ false };
            uint32_t numProjectiles{ 0 };

        public:
            Projectile(entt::registry& registry);
            virtual bool ProcessEvent(Event& evt) override;
            virtual void Update(double ts) override;
            virtual void RenderScene(glm::mat4 mvp) override;
            virtual void OnAttach() override;

        private:
            void SpawnProjectile();
        };
    }
}