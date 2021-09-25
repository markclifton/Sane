#pragma once

#include "sane/layers/base.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    class Console : public Layer
    {
    public:
        Console();
        virtual void Render() override;

    private:
        const Logging::LogSink logSink_{ Logging::GetLogSink() };
    };
}