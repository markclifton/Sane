#pragma once

#include "sane/ecs/common.hpp"
#include "sane/events/common.hpp"
#include "sane/systems/ecs/base.hpp"

namespace Sane
{
    namespace ECS
    {
        class Loader : public SystemBase, public Events::Listener
        {
        public:
            Loader(entt::registry& registry);
            virtual bool ProcessEvent(Event& event) override;

        private:
            bool LoadFile(const char* filepath);
        };
    }
}