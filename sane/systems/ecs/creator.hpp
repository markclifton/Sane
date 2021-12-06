#pragma once

#include "sane/ecs/common.hpp"
#include "sane/events/common.hpp"
#include "sane/systems/ecs/base.hpp"

namespace Sane
{
    namespace ECS
    {
        class Creator : public SystemBase, public Events::Listener
        {
        public:
            Creator(entt::registry& registry);

            virtual bool ProcessEvent(Event& event) override;

            bool LoadFile(const char* filepath);
        };
    }
}
