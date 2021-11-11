#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <string>

#include "sane/systems/stack.hpp"

namespace Sane
{
    class System
    {
        friend class Systems::Stack;
    public:
        System(const std::string& name);

        virtual void Update(double ts);
        virtual void RenderScene(glm::mat4 mvp);
        virtual void RenderGui();

        virtual void OnAttach();
        virtual void OnDetach();

        std::string name;
    };
}