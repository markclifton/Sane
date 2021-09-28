#pragma once

#include "sane/layers/layer.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    class Console : public Layer
    {
    public:
        Console();
        virtual void Update() override;

    private:
        const Logging::Sink sink_;
    };
}