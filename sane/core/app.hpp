#pragma once

#include <entt/entt.hpp>

#include "sane/core/display.hpp"
#include "sane/systems/system.hpp"
#include "sane/events/queue.hpp"
#include "sane/graphics/framebuffer.hpp"

#include "sane/systems/common.hpp"

namespace Sane
{
    class App
    {
    public:
        App(const char* name);
        virtual ~App() = default;
        void Run();

        void PushLayer(System* layer);
        void PushOverlay(System* layer);
        void PopLayer(System* layer);
        void PopOverlay(System* layer);

        entt::registry& Registry() { return registry_; }

        void DisplayConsole(bool Enable)
        {
            if (Enable)
                PushOverlay(&console);
            else
                PopOverlay(&console);
        }

    private:
        Console console;

        entt::registry registry_;
        Events::Queue& evt_queue_;
        Systems::Stack layers_;

        Display display_;
        ImguiBeginLayer imguiBegin_;
        ImguiEndLayer imguiEnd_;

    protected:
        Framebuffer framebuffer;
    };

    App* CreateApp();
}