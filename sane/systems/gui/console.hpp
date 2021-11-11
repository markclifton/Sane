#pragma once

#include "sane/logging/log.hpp"
#include "sane/systems/system.hpp"

namespace Sane
{
    class Console : public System
    {
        const Logging::Sink sink_;
    public:
        Console();
        virtual void RenderGui() override;
    };
}