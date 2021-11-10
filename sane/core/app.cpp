#include "app.hpp"

#include <filesystem>

#ifdef WIN32
#include <windows.h>
#endif

#include "sane/logging/log.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "sane/ecs/common.hpp"

const int32_t WIDTH = 1280;
const int32_t HEIGHT = 720;

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
        Systems::Scene scene(Registry());
        {
            const auto camera0 = Registry().create();
            Registry().emplace<Sane::Components::Camera>(camera0, true);
            Registry().emplace<Sane::Components::Position>(camera0, 2.f, -2.f, 0.f, 0.f);
            Registry().emplace<Sane::Components::Rotation>(camera0, 0.f, 0.f, 0.f);
            Registry().emplace<Sane::Components::RenderContext>(camera0, 2 * WIDTH, 2 * HEIGHT);
        }

        Systems::Camera camSystem(Registry());
        PushSystem(&camSystem);
        PushSystem(&scene);

        double last = glfwGetTime();
        while (display_.IsRunning())
        {
            framebuffer.Bind();
            framebuffer.Clear();

            for (auto& system : systems_)
            {
                system->Update((glfwGetTime() - last) * 1000);
            }
            last = glfwGetTime();
            framebuffer.Unbind();

            for (Layer* layer : layers_)
            {
                layer->Update();
            }

            display_.Update();
        }
    }

    void App::PushLayer(Layer* layer)
    {
        layers_.PushLayer(layer);
        SANE_INFO("Pushed layer on stack: {}", layer->Name());
    }

    void App::PushOverlay(Layer* layer)
    {
        layers_.PushOverlay(layer);
        SANE_INFO("Pushed overlay on stack: {}", layer->Name());
    }

    void App::PopLayer(Layer* layer)
    {
        layers_.PopLayer(layer);
    }

    void App::PopOverlay(Layer* layer)
    {
        layers_.PopOverlay(layer);
    }
}