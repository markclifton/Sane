#pragma once

#include <entt/entt.hpp>
#include <vector>

#include "sane/core/display.hpp"
#include "sane/ecs/systems/system.hpp"
#include "sane/events/queue.hpp"
#include "sane/graphics/framebuffer.hpp"
#include "sane/layers/console.hpp"
#include "sane/layers/gamewindow.hpp"
#include "sane/layers/imgui.hpp"
#include "sane/layers/stack.hpp"

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

        entt::registry& Registry() { return registry_; }
        void PushSystem(Systems::System* system) { systems_.push_back(system); }

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
        Layers::Stack layers_;

        Display display_;
        ImguiBeginLayer imguiBegin_;
        ImguiEndLayer imguiEnd_;

        std::vector<Systems::System*> systems_;

    protected:
        Framebuffer framebuffer;
    };

    App* CreateApp();
}