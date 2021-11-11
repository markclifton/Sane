#include "sane/systems/ecs/base.hpp"

namespace Sane
{
    namespace ECS
    {
        SystemBase::SystemBase(const std::string& name, entt::registry& registry)
            : System(name)
            , registry_(registry)
        {}
    }
}