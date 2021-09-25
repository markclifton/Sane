#pragma once

#include "sane/events/event.hpp"
#include "sane/layers/layer.hpp"

namespace Sane
{
    class App
    {
    public:
        App();
        virtual ~App() = default;
        virtual void Run() = 0;

    protected:
        Sane::LayerStack layers_;
        Sane::LayerStack overlay_layers_;
        Sane::Events::Queue& evt_queue_;
    };

    App* CreateApp();
}