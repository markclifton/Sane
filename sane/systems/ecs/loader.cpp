#include "sane/systems/ecs/loader.hpp"

#include <tinyxml2.h>

#include "sane/logging/log.hpp"

namespace Sane
{
    namespace ECS
    {
        Loader::Loader(entt::registry& registry)
            : SystemBase("LoaderSystem", registry)
            , Events::Listener("LoaderSystem")
        {
        }

        bool Loader::ProcessEvent(Event& event)
        {
            if (event.action == kLoadEvent)
            {
                LoadEvent& loadEvt = *(LoadEvent*)event.data;
                return LoadFile(loadEvt.filepath.c_str());
            }

            return false;
        }

        bool Loader::LoadFile(const char* filepath)
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