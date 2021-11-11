#pragma once

#include "sane/systems/system.hpp"

namespace Sane
{
    class FpsCounter : public System
    {
    public:
        FpsCounter();
        virtual void RenderGui() override;
    };
}