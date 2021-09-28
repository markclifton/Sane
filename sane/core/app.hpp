#pragma once

#include "sane/core/display.hpp"
#include "sane/events/queue.hpp"
#include "sane/graphics/framebuffer.hpp"
#include "sane/layers/stack.hpp"
#include "sane/layers/imgui.hpp"
#include "sane/layers/gamewindow.hpp"

namespace Sane
{
    class App
    {
        const int32_t WIDTH = 1280;
        const int32_t HEIGHT = 720;
    public:
        App(const char* name);
        virtual ~App() = default;
        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

    private:
        Events::Queue& evt_queue_;
        Layers::Stack layers_;

        Display display_;
        ImguiBeginLayer imguiBegin_;
        ImguiEndLayer imguiEnd_;

    protected:
        Framebuffer framebuffer;
    };

    App* CreateApp();
}