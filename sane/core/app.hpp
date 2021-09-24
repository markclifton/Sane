#pragma once

#include <sane/core/layer.hpp>
#include <sane/core/event.hpp>
#include <sane/debugging/logging.hpp>

namespace Sane
{
    class App
    {
    public:
        App();
        virtual ~App() = default;
        virtual void Run() = 0;

    protected:
        Sane::LayerStack layers;
        Sane::EventQueue& evt_queue;
    };

    App* CreateApp();
}