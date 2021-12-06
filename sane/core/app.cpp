#include "app.hpp"

#include <filesystem>

#ifdef WIN32
#include <windows.h>
#endif

#include "sane/ecs/common.hpp"
#include "sane/logging/log.hpp"
#include "sane/systems/common.hpp"
#include "sane/systems/ecs/scene.hpp"

const int32_t WIDTH = 2560;
const int32_t HEIGHT = 1440;

namespace Sane
{
    App::App(const char* name)
        : evt_queue_(Events::Queue::Instance())
        , display_(name, WIDTH, HEIGHT)
        , imguiBegin_(&display_)
        , imguiEnd_(&display_)
        , framebuffer(WIDTH, HEIGHT)
    {
#if defined(WIN32)
        FreeConsole();
#endif

        PushLayer(&evt_queue_);
        layers_.PushImguiIntegration(&imguiBegin_, &imguiEnd_);
        SANE_WARN("Current path is {}", std::filesystem::current_path().string());
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
        SANE_INFO("Pushed layer on stack: {}", layer->name);
    }

    void App::PushOverlay(System* layer)
    {
        layers_.PushOverlay(layer);
        SANE_INFO("Pushed overlay on stack: {}", layer->name);
    }

    void App::PopLayer(System* layer)
    {
        layers_.PopLayer(layer);
    }

    void App::PopOverlay(System* layer)
    {
        layers_.PopOverlay(layer);
    }
}