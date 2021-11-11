#pragma once

#include <entt/entt.hpp>

#include "sane/systems/system.hpp"

namespace Sane
{
    namespace ECS
    {
        class SystemBase : public System
        {
        protected:
            entt::registry& registry_;
        public:
            SystemBase(const std::string& name, entt::registry& registry);
        };
    }
}