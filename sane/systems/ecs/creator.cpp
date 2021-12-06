#include "sane/systems/ecs/creator.hpp"

#include <tinyxml2.h>

#include "sane/logging/log.hpp"

namespace Sane
{
    namespace ECS
    {
        Creator::Creator(entt::registry& registry)
            : SystemBase("CreatorSystem", registry)
            , Events::Listener("CreatorSystem")
        {
        }

        bool Creator::ProcessEvent(Event& event)
        {
            if (event.action == kLoadEvent)
            {
                LoadEvent& loadEvt = *(LoadEvent*)event.data;
                return LoadFile(loadEvt.filepath.c_str());
            }

            return false;
        }

        bool Creator::LoadFile(const char* filepath)
        {
            tinyxml2::XMLDocument doc;
            if (doc.LoadFile(filepath) != tinyxml2::XMLError::XML_SUCCESS)
            {
                SANE_ERROR("Failed to load: {}", filepath);
                return false;
            }

            if (!doc.FirstChild())
            {
                SANE_ERROR("Failed to parse: {}", filepath);
                return false;
            }

            SANE_INFO("XML First: {}", doc.FirstChild()->Value());
            return true;
        }
    }
}