#pragma once

#include <cstdint>
#include <entt/entt.hpp>

namespace Sane
{
    namespace Systems
    {
        class System
        {
        public:
            System(entt::registry& registry) : registry_(registry) {}

            virtual void Update(uint64_t ts) = 0;

        protected:
            entt::registry& registry_;
        };
    }
}