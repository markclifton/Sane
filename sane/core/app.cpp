#include "app.hpp"

#include <filesystem>

#ifdef WIN32
#include <windows.h>
#endif

#include "sane/ecs/common.hpp"
#include "sane/logging/log.hpp"
#include "sane/systems/common.hpp"

namespace Sane
{
    App::App(const char* name, uint32_t width, uint32_t height)
        : evt_queue_(Events::Queue::Instance())
        , display_(name, width, height)
        , imguiBegin_(&display_)
        , imguiEnd_(&display_)
        , framebuffer(width, height)
        , gameWindow(GetColorAttachment())
    {
#if defined(WIN32)
        FreeConsole();
#endif

        PushLayer(&evt_queue_);
        layers_.PushImguiIntegration(&imguiBegin_, &imguiEnd_);

        SANE_WARN("CWD: {}", std::filesystem::current_path().string());

        PushOverlay(&gameWindow);
    }

    void App::Run()
    {
        double last = glfwGetTime();
        while (display_.IsRunning())
        {
            for (auto& layer : layers_)
            {
                layer->Update((glfwGetTime() - last) * 1000);
            }
            last = glfwGetTime();

            framebuffer.Bind();
            framebuffer.Clear();
            for (auto& layer : layers_)
            {
                layer->RenderScene(display_.GetPersProjection());
            }
            framebuffer.Unbind();

            for (auto& layer : layers_)
            {
                layer->RenderGui();
            }

            display_.Update();
        }
    }

    void App::PushLayer(System* layer)
    {
        layers_.PushLayer(layer);
        SANE_DEBUG("Pushed layer on stack: {}", layer->name);
    }

    void App::PushOverlay(System* layer)
    {
        layers_.PushOverlay(layer);
        SANE_DEBUG("Pushed overlay on stack: {}", layer->name);
    }

    void App::PopLayer(System* layer)
    {
        layers_.PopLayer(layer);
    }

    void App::PopOverlay(System* layer)
    {
        layers_.PopOverlay(layer);
    }

    entt::registry& App::Registry()
    {
        return registry_;
    }

    void App::DisplayConsole(bool Enable)
    {
#ifndef NDEBUG
        if (Enable)
            PushOverlay(&console);
        else
            PopOverlay(&console);
#endif
    }

    uint32_t App::GetColorAttachment()
    {
        return framebuffer.GetAttachment(0);
    }

}