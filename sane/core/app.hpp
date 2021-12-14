#pragma once

#include <entt/entt.hpp>

#include "sane/core/display.hpp"
#include "sane/events/common.hpp"
#include "sane/graphics/common.hpp"
#include "sane/systems/common.hpp"

namespace Sane
{
    class App : public Events::Dispatcher
    {
    public:
        App(const char* name, uint32_t width, uint32_t height);
        virtual ~App() = default;
        void Run();

        void PushLayer(System* layer);
        void PushOverlay(System* layer);
        void PopLayer(System* layer);
        void PopOverlay(System* layer);

        entt::registry& Registry();
        void DisplayConsole(bool Enable);
        uint32_t GetColorAttachment();

    private:
#ifndef NDEBUG
        Console console;
#endif
        entt::registry registry_;
        Events::Queue& evt_queue_;
        Systems::Stack layers_;

        Display display_;
        ImguiBeginLayer imguiBegin_;
        ImguiEndLayer imguiEnd_;

        Framebuffer framebuffer;
        GameWindow gameWindow;
    };

    App* CreateApp();
}