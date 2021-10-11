#pragma once

#include "sane/ecs/common.hpp"
#include "sane/events/common.hpp"

namespace Sane
{
    namespace Systems
    {
        class Movement : public System, public Events::Listener
        {
            float x{ 0 }, y{ 0 }, z{ 0 };
        public:
            Movement(entt::registry& registry);
            virtual bool ProcessEvent(Event& event) override;
            virtual void Update(uint64_t ts) override;
        };
    }
}