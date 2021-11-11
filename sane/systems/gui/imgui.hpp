#pragma once

#include "sane/core/display.hpp"
#include "sane/systems/system.hpp"

namespace Sane
{
    class ImguiBeginLayer : public System
    {
        Sane::Display* display_;

    public:
        ImguiBeginLayer(Display* display);
        virtual void RenderGui() override;
        virtual void OnAttach() override;
        virtual void OnDetach() override;
    };

    class ImguiEndLayer : public System
    {
        Sane::Display* display_;
    public:
        ImguiEndLayer(Display* display);
        virtual void RenderGui() override;
    };
}