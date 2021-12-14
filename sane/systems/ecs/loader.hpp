#pragma once

#include "sane/ecs/common.hpp"
#include "sane/events/common.hpp"
#include "sane/systems/common.hpp"

namespace tinyxml2 { class XMLNode; }

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
            bool LoadModels(tinyxml2::XMLNode* node);
            bool LoadObjects(tinyxml2::XMLNode* node);
            bool LoadSounds(tinyxml2::XMLNode* node);
            bool LoadTextures(tinyxml2::XMLNode* node);
        };
    }
}