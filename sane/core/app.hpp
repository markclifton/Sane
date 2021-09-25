#pragma once

#include "sane/core/display.hpp"
#include "sane/events/event.hpp"
#include "sane/layers/base.hpp"
#include "sane/layers/imgui.hpp"

namespace Sane
{
    class App
    {
    public:
        App(const char* name);
        virtual ~App() = default;
        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

    private:
        const size_t WIDTH = 1280;
        const size_t HEIGHT = 720;

        Events::Queue& evt_queue_;
        Layers::Stack layers_;

        Display display_;
        ImguiIntegration imgui_;
    };

    App* CreateApp();
}