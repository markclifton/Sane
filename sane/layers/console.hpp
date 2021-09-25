#pragma once

#include "sane/layers/layer.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    class Console : public Layer
    {
    public:
        virtual void Process() override;

    private:
        const Logging::LogSink logSink_{ Logging::GetLogSink() };
    };
}