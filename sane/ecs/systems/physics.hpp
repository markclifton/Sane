#pragma once

#include "sane/ecs/systems/system.hpp"

#include "sane/ecs/components/gravity.hpp"
#include "sane/ecs/components/translation.hpp"

#include "sane/logging/log.hpp"

namespace Sane
{
    namespace Systems
    {
        class Physics : public System
        {
        public:
            Physics(entt::registry& registry)
                : System(registry)
            {}

            virtual void Update(uint64_t ts) override
            {
                auto view = registry_.view<Components::Gravity, Components::Translation>();
                view.each([](const auto entity, const auto& gravity, auto& t) {
                    t.y -= gravity.constant;
                    });
            }

        private:
        };
    }
}