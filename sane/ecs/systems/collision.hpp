#pragma once

#include <algorithm>
#include <cmath>
#include <functional>

#include "sane/ecs/components/collidable.hpp"
#include "sane/ecs/components/position.hpp"
#include "sane/ecs/components/translation.hpp"
#include "sane/ecs/systems/system.hpp"
#include "sane/math/vector.hpp"

#include "sane/logging/log.hpp"

namespace Sane
{
    namespace Systems
    {
        class Collision : public System
        {
        public:
            Collision(entt::registry& registry)
                : System(registry)
            {}

            virtual void Update(uint64_t ts) override
            {
            }

        private:
        };
    }
}