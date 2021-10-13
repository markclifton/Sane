#pragma once

#include <chrono>

namespace Sane
{
    class Timer
    {
    public:
        Timer();
        double reset();
        double get();

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_lastTick;
    };
}