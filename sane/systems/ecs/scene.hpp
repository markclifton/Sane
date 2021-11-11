#pragma once

#include "sane/systems/ecs/base.hpp"

namespace Sane
{
    namespace ECS
    {
        class Scene : public SystemBase
        {
        public:
            Scene(entt::registry& registry);
            virtual void RenderScene(glm::mat4 mvp) override;
        };
    }
}
