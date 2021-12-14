#include "sane/systems/ecs/loader.hpp"

#include <functional>
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
                std::string path{ loadEvt.filepath, loadEvt.size };

                tinyxml2::XMLDocument doc;
                if (doc.LoadFile(path.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
                {
                    SANE_ERROR("Failed to load: {}", path);
                    return false;
                }

                if (!doc.FirstChild())
                {
                    SANE_ERROR("Failed to parse: {}", path);
                    return false;
                }

                return LoadModels(doc.FirstChild()->FirstChildElement("models"))
                    || LoadObjects(doc.FirstChild()->FirstChildElement("objects"))
                    || LoadSounds(doc.FirstChild()->FirstChildElement("sounds"))
                    || LoadTextures(doc.FirstChild()->FirstChildElement("textures"));
            }
            return false;
        }

        bool Loader::LoadModels(tinyxml2::XMLNode* node)
        {
            return false;
        }

        bool Loader::LoadObjects(tinyxml2::XMLNode* node)
        {

            if (!node)
                return false;

            tinyxml2::XMLNode* currentNode = node->FirstChild();
            while (currentNode)
            {
                if (strcmp(currentNode->ToElement()->Attribute("type"), "player") == 0)
                {
                    auto pos = currentNode->FirstChildElement("position");
                    auto posElement = pos->ToElement();

                    auto rot = currentNode->FirstChildElement("rotation");
                    auto rotElement = rot->ToElement();

                    const auto camera0 = registry_.create();
                    registry_.emplace<Sane::Components::Camera>(camera0, true, glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f), 10.0f);
                    registry_.emplace<Sane::Components::Position>(camera0, glm::vec4(std::stof(posElement->Attribute("x")), std::stof(posElement->Attribute("y")), std::stof(posElement->Attribute("z")), 0.f));
                    registry_.emplace<Sane::Components::Rotation>(camera0, std::stof(rotElement->Attribute("x")), std::stof(rotElement->Attribute("y")), std::stof(rotElement->Attribute("z")));
                    registry_.emplace<Sane::Components::RenderContext>(camera0, 1920, 1080);
                    registry_.emplace<Sane::Components::Player>(camera0, true);
                }

                currentNode = currentNode->NextSibling();
            }

            return true;
        }

        bool Loader::LoadSounds(tinyxml2::XMLNode* node)
        {
            return false;
        }

        bool Loader::LoadTextures(tinyxml2::XMLNode* node)
        {
            return false;
        }
    }
}